/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"
#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreRoot.h"

namespace Ogre {
	//---------------------------------------------------------------------
	DefaultIntersectionSceneQuery::DefaultIntersectionSceneQuery(SceneManager* creator)
	: IntersectionSceneQuery(creator)
	{
		// No world geometry results supported
		mSupportedWorldFragments.insert(SceneQuery::WFT_NONE);
	}
	//---------------------------------------------------------------------
	DefaultIntersectionSceneQuery::~DefaultIntersectionSceneQuery()
	{
	}
	//---------------------------------------------------------------------
	void DefaultIntersectionSceneQuery::execute(IntersectionSceneQueryListener* listener)
	{
		// Iterate over all movable types
		Root::MovableObjectFactoryIterator factIt = 
			Root::getSingleton().getMovableObjectFactoryIterator();
		while(factIt.hasMoreElements())
		{
			SceneManager::MovableObjectIterator objItA = 
				mParentSceneMgr->getMovableObjectIterator(
					factIt.getNext()->getType());
			while (objItA.hasMoreElements())
			{
				MovableObject* a = objItA.getNext();
				// skip entire section if type doesn't match
				if (!(a->getTypeFlags() & mQueryTypeMask))
					break;

				// Skip if a does not pass the mask
				if (!(a->getQueryFlags() & mQueryMask) ||
					!a->isInScene())
					continue;

				// Check against later objects in the same group
				SceneManager::MovableObjectIterator objItB = objItA;
				while (objItB.hasMoreElements())
				{
					MovableObject* b = objItB.getNext();

					// Apply mask to b (both must pass)
					if ((b->getQueryFlags() & mQueryMask) && 
						b->isInScene())
					{
						const AxisAlignedBox& box1 = a->getWorldBoundingBox();
						const AxisAlignedBox& box2 = b->getWorldBoundingBox();

						if (box1.intersects(box2))
						{
							if (!listener->queryResult(a, b)) return;
						}
					}
				}
				// Check  against later groups
				Root::MovableObjectFactoryIterator factItLater = factIt;
				while (factItLater.hasMoreElements())
				{
					SceneManager::MovableObjectIterator objItC = 
						mParentSceneMgr->getMovableObjectIterator(
							factItLater.getNext()->getType());
					while (objItC.hasMoreElements())
					{
						MovableObject* c = objItC.getNext();
						// skip entire section if type doesn't match
						if (!(c->getTypeFlags() & mQueryTypeMask))
							break;

						// Apply mask to c (both must pass)
						if ((c->getQueryFlags() & mQueryMask) &&
							c->isInScene())
						{
							const AxisAlignedBox& box1 = a->getWorldBoundingBox();
							const AxisAlignedBox& box2 = c->getWorldBoundingBox();

							if (box1.intersects(box2))
							{
								if (!listener->queryResult(a, c)) return;
							}
						}
					}

				}

			}


		}

	}
	//---------------------------------------------------------------------
	DefaultAxisAlignedBoxSceneQuery::
	DefaultAxisAlignedBoxSceneQuery(SceneManager* creator)
	: AxisAlignedBoxSceneQuery(creator)
	{
		// No world geometry results supported
		mSupportedWorldFragments.insert(SceneQuery::WFT_NONE);
	}
	//---------------------------------------------------------------------
	DefaultAxisAlignedBoxSceneQuery::~DefaultAxisAlignedBoxSceneQuery()
	{
	}
	//---------------------------------------------------------------------
	void DefaultAxisAlignedBoxSceneQuery::execute(SceneQueryListener* listener)
	{
		// Iterate over all movable types
		Root::MovableObjectFactoryIterator factIt = 
			Root::getSingleton().getMovableObjectFactoryIterator();
		while(factIt.hasMoreElements())
		{
			SceneManager::MovableObjectIterator objItA = 
				mParentSceneMgr->getMovableObjectIterator(
				factIt.getNext()->getType());
			while (objItA.hasMoreElements())
			{
				MovableObject* a = objItA.getNext();
				// skip whole group if type doesn't match
				if (!(a->getTypeFlags() & mQueryTypeMask))
					break;

				if ((a->getQueryFlags() & mQueryMask) && 
					a->isInScene() &&
					mAABB.intersects(a->getWorldBoundingBox()))
				{
					if (!listener->queryResult(a)) return;
				}
			}
		}
	}
	//---------------------------------------------------------------------
	DefaultRaySceneQuery::
	DefaultRaySceneQuery(SceneManager* creator) : RaySceneQuery(creator)
	{
		// No world geometry results supported
		mSupportedWorldFragments.insert(SceneQuery::WFT_NONE);
	}
	//---------------------------------------------------------------------
	DefaultRaySceneQuery::~DefaultRaySceneQuery()
	{
	}
	//---------------------------------------------------------------------
	void DefaultRaySceneQuery::execute(RaySceneQueryListener* listener)
	{
		// Note that becuase we have no scene partitioning, we actually
		// perform a complete scene search even if restricted results are
		// requested; smarter scene manager queries can utilise the paritioning 
		// of the scene in order to reduce the number of intersection tests 
		// required to fulfil the query

		// Iterate over all movable types
		Root::MovableObjectFactoryIterator factIt = 
			Root::getSingleton().getMovableObjectFactoryIterator();
		while(factIt.hasMoreElements())
		{
			SceneManager::MovableObjectIterator objItA = 
				mParentSceneMgr->getMovableObjectIterator(
				factIt.getNext()->getType());
			while (objItA.hasMoreElements())
			{
				MovableObject* a = objItA.getNext();
				// skip whole group if type doesn't match
				if (!(a->getTypeFlags() & mQueryTypeMask))
					break;

				if( (a->getQueryFlags() & mQueryMask) &&
					a->isInScene())
				{
					// Do ray / box test
					std::pair<bool, Real> result =
						mRay.intersects(a->getWorldBoundingBox());

					if (result.first)
					{
						if (!listener->queryResult(a, result.second)) return;
					}
				}
			}
		}

	}
	//---------------------------------------------------------------------
	DefaultSphereSceneQuery::
	DefaultSphereSceneQuery(SceneManager* creator) : SphereSceneQuery(creator)
	{
		// No world geometry results supported
		mSupportedWorldFragments.insert(SceneQuery::WFT_NONE);
	}
	//---------------------------------------------------------------------
	DefaultSphereSceneQuery::~DefaultSphereSceneQuery()
	{
	}
	//---------------------------------------------------------------------
	void DefaultSphereSceneQuery::execute(SceneQueryListener* listener)
	{
		Sphere testSphere;

		// Iterate over all movable types
		Root::MovableObjectFactoryIterator factIt = 
			Root::getSingleton().getMovableObjectFactoryIterator();
		while(factIt.hasMoreElements())
		{
			SceneManager::MovableObjectIterator objItA = 
				mParentSceneMgr->getMovableObjectIterator(
				factIt.getNext()->getType());
			while (objItA.hasMoreElements())
			{
				MovableObject* a = objItA.getNext();
				// skip whole group if type doesn't match
				if (!(a->getTypeFlags() & mQueryTypeMask))
					break;
				// Skip unattached
				if (!a->isInScene() || 
					!(a->getQueryFlags() & mQueryMask))
					continue;

				// Do sphere / sphere test
				testSphere.setCenter(a->getParentNode()->_getDerivedPosition());
				testSphere.setRadius(a->getBoundingRadius());
				if (mSphere.intersects(testSphere))
				{
					if (!listener->queryResult(a)) return;
				}
			}
		}
	}
	//---------------------------------------------------------------------
	DefaultPlaneBoundedVolumeListSceneQuery::
	DefaultPlaneBoundedVolumeListSceneQuery(SceneManager* creator) 
	: PlaneBoundedVolumeListSceneQuery(creator)
	{
		// No world geometry results supported
		mSupportedWorldFragments.insert(SceneQuery::WFT_NONE);
	}
	//---------------------------------------------------------------------
	DefaultPlaneBoundedVolumeListSceneQuery::~DefaultPlaneBoundedVolumeListSceneQuery()
	{
	}
	//---------------------------------------------------------------------
	void DefaultPlaneBoundedVolumeListSceneQuery::execute(SceneQueryListener* listener)
	{
		// Iterate over all movable types
		Root::MovableObjectFactoryIterator factIt = 
			Root::getSingleton().getMovableObjectFactoryIterator();
		while(factIt.hasMoreElements())
		{
			SceneManager::MovableObjectIterator objItA = 
				mParentSceneMgr->getMovableObjectIterator(
				factIt.getNext()->getType());
			while (objItA.hasMoreElements())
			{
				MovableObject* a = objItA.getNext();
				// skip whole group if type doesn't match
				if (!(a->getTypeFlags() & mQueryTypeMask))
					break;

				PlaneBoundedVolumeList::iterator pi, piend;
				piend = mVolumes.end();
				for (pi = mVolumes.begin(); pi != piend; ++pi)
				{
					PlaneBoundedVolume& vol = *pi;
					// Do AABB / plane volume test
					if ((a->getQueryFlags() & mQueryMask) && 
						a->isInScene() && 
						vol.intersects(a->getWorldBoundingBox()))
					{
						if (!listener->queryResult(a)) return;
						break;
					}
				}
			}
		}
	}
}
