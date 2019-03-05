#include "j1Entity.h"
#include "j1Group.h"
#include "j1MovementManager.h"
#include "j1App.h"
#include "j1EntityManager.h" 
#include "p2Log.h"

j1MovementManager::j1MovementManager()
{
}

j1MovementManager::~j1MovementManager()
{
}

bool j1MovementManager::Update(float dt)
{
	return true;
}

bool j1MovementManager::CleanUp()
{
	std::list <j1Group*>::const_iterator group = Groups.begin();

	while (group != Groups.end())
	{
		(*group)->ClearGroup();
		delete *group;
		group++;
	}

	Groups.clear();

	return true;
}

void j1MovementManager::SelectEntities_inRect(SDL_Rect SRect)
{
	std::list<j1Entity*>::iterator entity = App->manager->entities.begin();
	SDL_Rect entityrect = { 0,0,0,0 };

	while (entity != App->manager->entities.end())
	{
		entityrect = { (int)(*entity)->info.position.x, (int)(*entity)->info.position.y, (*entity)->info.Size.x, (*entity)->info.Size.y };

		// --- Check entity's rect against the given SRect, select it if overlap is positive ---
		if (SDL_HasIntersection(&entityrect, &SRect))
			(*entity)->info.IsSelected = true;
		else
			(*entity)->info.IsSelected = false;

		entity++;
	}

}

void j1MovementManager::CreateGroup()
{
	bool Validgroup = false;

	j1Group* group = new j1Group;

	std::list<j1Entity*>::iterator entity = App->manager->entities.begin();

	while (entity != App->manager->entities.end())
	{

		// --- Check for entities that have been selected ---
		if ((*entity)->info.IsSelected)
		{
			// --- If we find an entity then the group is valid and can be created ---
			Validgroup = true;

			// --- Remove the entity from a previous group if any is found ---
			if ((*entity)->current_group != nullptr)
			{
				(*entity)->current_group->removeUnit(*entity);

				if ((*entity)->current_group->GetSize() == 0)
					Groups.remove((*entity)->current_group);
			}

			// --- Add the entity to the new group, update its current group pointer ---
			group->addUnit(*entity);
			(*entity)->current_group = group;
		}

		entity++;
	}

	// --- Finally, If the group is Valid add it to our Groups list, else delete it ---
	if (Validgroup)
		Groups.push_back(group);
	else
		delete group;

//	LOG("Groups size %i", Groups.size());
}

void j1MovementManager::Move(j1Group * group, float dt)
{
	std::list <j1Entity*>::const_iterator unit = group->Units.begin();

	LOG("On Move Function");

	while (unit != group->Units.end())
	{

		switch ((*unit)->UnitMovementState)
		{

		case MovementState::MovementState_NoState:

			// --- On call to Move, Units will request a path to the destination ---

			// --- If any other unit of the group has the same goal, change the goal tile ---

			break;

		case MovementState::MovementState_Wait:

			// --- Whenever the unit faces an obstacle of any type during a scheduled path, overcome it ---

			break;

		case MovementState::MovementState_FollowPath:

			// --- If a path is created, the unit will start following it ---

			break;

		case MovementState::MovementState_NextStep:

			// --- If a path is being followed, the unit will get the next tile in the path and advance ---

			break;

		case MovementState::MovementState_DestinationReached:

			// --- The unit reaches the end of the path, thus stopping and returning to NoState ---

			break;
		}

		unit++;

	}

}