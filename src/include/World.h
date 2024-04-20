﻿#pragma once

#include <set>
#include <unordered_map>
#include <fstream>

#include "Abstract_entities.h"
#include "pair_hash.h"
#include "randomness.h"


#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <pplx/pplxtasks.h>
#include <concurrent_unordered_map.h>

#include <random>
#include <shared_mutex>

class Entity;



#define MAX_ENTITIES 100

class World;



class World
{
	std::unordered_multimap<std::pair<int, int>, Entity*, pair_hash>* world_map;
	//std::unordered_multimap<std::pair<int, int>, Entity*, pair_hash>*;
	//std::unordered_set<Entity*>* removed_entities;

	std::vector<std::pair<Entity*,pplx::task<void>>> entity_tasks;

	pplx::cancellation_token_source cts;
	Concurrency::cancellation_token token = cts.get_token();
	
	//concurrency::concurrent_unordered_multimap<std::pair<int,int>,Entity*,pair_hash> world_map_conc;

	unsigned short ticks = 10;
protected:

public:
	std::shared_mutex world_map_mutex;
	
private:
	void thread_entity(Entity* ent)
	{
		Animal* this_animal = dynamic_cast<Animal*>(ent);
		if (this_animal == nullptr) return;

		while (!token.is_canceled())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			this_animal->age_up();

			std::list<Entity*> entities;

			world_map_mutex.lock();

			// skan w odleglosci 1
			/*
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first, an->get_position().second)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first + 1, an->get_position().second)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first, an->get_position().second + 1)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first + 1, an->get_position().second + 1)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first - 1, an->get_position().second)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first, an->get_position().second - 1)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first - 1, an->get_position().second - 1)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first - 1, an->get_position().second + 1)));
			entities.emplace_back(get_entity(std::make_pair(an->get_position().first + 1, an->get_position().second - 1)));
			*/

			world_map_mutex.unlock();

			const int dice = roll_dice_100();

			if (dice > 50)	// movement action
			{
				std::lock_guard<std::shared_mutex> lock(world_map_mutex);

				int where = roll_dice_100();

				if (where > 7 * 100 / 8)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first + 1, this_animal->get_position().second));

				}
				else if (where > 6 * 100 / 8)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first, this_animal->get_position().second + 1));

				}
				else if (where > 5 * 100 / 8)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first - 1, this_animal->get_position().second));

				}
				else if (where > 4 * 100 / 8)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first, this_animal->get_position().second - 1));

				}
				else if (where > 3 * 100 / 8)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first + 1, this_animal->get_position().second + 1));

				}
				else if (where > 2 * 100 / 8)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first - 1, this_animal->get_position().second - 1));

				}
				else if (where > 1 * 100 / 8)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first - 1, this_animal->get_position().second + 1));

				}
				else if (where > 0)
				{
					move_entity(this_animal, std::make_pair(this_animal->get_position().first + 1, this_animal->get_position().second - 1));
				}
			}
			else				// rozmnozenie
			{
				Entity* second_target = nullptr;

				world_map_mutex.lock();
				auto values_it = world_map->equal_range(this_animal->get_position());

				for (auto it = values_it.first; it != values_it.second; it++)
				{
					Organism* org = dynamic_cast<Organism*>(it->second);
					if (org != this_animal) 
					{
						second_target = org;
						break;
					}
				}

				if (second_target != nullptr && !dynamic_cast<Animal*>(second_target)->recently_reproduced() && !this_animal->recently_reproduced())
				{
					Animal* an2 = dynamic_cast<Animal*>(second_target);
					Organism* new_animal = this_animal->bear_child(an2);
					this->add_entity(new_animal, new_animal->get_position());
					this->entity_tasks.emplace_back(new_animal,pplx::create_task([this,&new_animal]() {thread_entity(new_animal); },token));
				}
				world_map_mutex.unlock();
			}
		}
	}

public:
	World()
	{
		world_map = new std::unordered_multimap<std::pair<int, int>, Entity*, pair_hash>;

#ifdef _DEBUG
		std::cout << "World created" << std::endl;
#endif
	}
	~World()
	{
		for (auto& [pos, ent] : *world_map)
		{
			delete ent;
		}

		#ifdef _DEBUG
			std::cout << "World destroyed" << std::endl;
		#endif
	}

	void add_entity(Entity* org, std::pair<int, int> pos) 
	{
		//std::scoped_lock<std::shared_mutex> lock(world_map_mutex);

		//world_map_mutex.lock();

		world_map->insert(std::make_pair(pos,org));
		org->set_position(pos);

		//world_map_mutex.unlock();

		#ifdef _DEBUG
		std::cout << "Entity added" << std::endl;
		#endif
	}

	/*void remove_entity(Entity* org)
	{
#ifdef _DEBUG
		std::cout << "Entity removed" << std::endl;
#endif

	}*/
	std::list<Entity*> in_range(std::pair<int,int> pos,unsigned int range) const
	{
		std::list<Entity*> entities;

		//std::shared_lock lock(world_map_mutex);
		//lock.lock();

		//
		//	TODO: Implement in_range
		//

		//lock.unlock();
		return entities;
	}

	bool move_entity(Entity* org,const std::pair<int, int> new_pos)
	{
		//std::lock_guard<std::shared_mutex> lock(world_map_mutex);

		//if(world_map->find(new_pos) == world_map->end() || world_map->find(new_pos)->second->is_solid()) return false;

		if(world_map->contains(new_pos) && world_map->find(new_pos)->second->is_solid()) return false;

		auto values_it =world_map->equal_range(org->get_position());
		
		for(auto it = values_it.first; it != values_it.second; it++)
		{
			if(org == it->second)
			{
				auto ptmp = world_map->extract(org->get_position());
				ptmp.mapped()->move(new_pos);
				ptmp.key() = new_pos;

				world_map->insert(std::move(ptmp));

				//it->second= nullptr;

				//world_map->insert(std::make_pair(org->get_position(),org));
				
#ifdef _DEBUG
				std::cout << "Entity moved to " << new_pos.first << " " << new_pos.second << std::endl;
#endif

				return true;
			}
		}
		return false;
	}

	Entity* get_entity(const std::pair<int, int> pos) const
	{
		return !world_map->contains(pos) ? nullptr : world_map->find(pos)->second;
	}

	Entity* operator[](const std::pair<int, int> pos) const
	{
		return get_entity(pos);
	}

	void start()
	{
		for (auto& [pos, ent] : *world_map)
		{
			entity_tasks.emplace_back(ent, pplx::create_task([&ent,this]() {thread_entity(ent); }, token));
			
		}
	}

	pplx::task<void> stop()
	{
		cts.cancel();
		return Concurrency::create_task([this]()
		{
			for (auto& [ent,task] : entity_tasks) task.wait();
			this->entity_tasks.clear();
			cts = pplx::cancellation_token_source();
			token = cts.get_token();
		});
	}

	void save(const std::string& filename)
	{
		std::ofstream ofs(filename);
		boost::archive::text_oarchive oa(ofs);
		oa << *this;
	}
	void load_this(const std::string& filename)
	{
		this->stop().wait();
		std::ifstream ifs(filename);
		boost::archive::text_iarchive ia(ifs);
		ia >> *this;
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		//std::scoped_lock<std::shared_mutex> lock(world_map_mutex);

		ar&* world_map;

		/*for (auto& [pos, ent] : *world_map)
		{
			//ar& pos;
			//ar& ent;
		}*/
	}
};