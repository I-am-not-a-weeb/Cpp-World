#include <iostream>
#include <unordered_map>
#include <utility>

#include <boost/container_hash/hash.hpp>

#include <pplx/pplxtasks.h>

#include "include/pair_hash.h"

#include "include/World.h"
#include "include/Abstract_entities.h"
#include <concurrent_unordered_map.h>

#include <random>
#include <Windows.h>


std::string world_filename{ "world.txt" };

int main()
{
	std::ios_base::sync_with_stdio(false);

	std::cin.tie(nullptr);
	std::cerr.tie(nullptr);

	World world;
	std::pair<int, int> pos_animal1(1, 1);
	Animal* animal1 = new Animal(&world,pos_animal1,{0,1,2,3});
	Animal* animal2 = new Animal(&world, pos_animal1,{1,3,2,1});
	Animal* animal3 = dynamic_cast<Animal*>(animal1->bear_child(animal2));
	Animal* animal4 = dynamic_cast<Animal*>(animal2->bear_child(animal3));
	Animal* animal5 = dynamic_cast<Animal*>(animal3->bear_child(animal1));
	Animal* animal6 = dynamic_cast<Animal*>(animal4->bear_child(animal2));
	Animal* animal7 = dynamic_cast<Animal*>(animal1->bear_child(animal2));

	world.add_entity(animal1, pos_animal1);
	world.add_entity(animal2, pos_animal1);
	world.add_entity(animal3, animal3->get_position());
	world.add_entity(animal4, animal4->get_position());
	world.add_entity(animal5, animal5->get_position());
	world.add_entity(animal6, animal6->get_position());
	world.add_entity(animal7, animal7->get_position());

	world.move_entity(animal1, { 2,2 });

	auto i = 0;

	std::wstring com;



	auto main = pplx::create_task([&world,&com] {
		while(true)
		{
			std::wcin >> com;
			if (com == L"exit")
			{
				world.stop().wait();
				break;
			}
			else if(com == L"pause")
			{
				world.stop().wait();
			}
			else if (com == L"unpause")
			{
				world.start();
			}
			else if(com == L"start")
			{
				world.start();
			}
			else if(com == L"save")
			{
				world.save(world_filename);
			}
			else if(com == L"load")
			{
				world.load_this(world_filename);
			}
		}
	});

	main.wait();
}
