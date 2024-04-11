#include <iostream>
#include <unordered_map>
#include <utility>

#include <boost/container_hash/hash.hpp>

#include <pplx/pplxtasks.h>

#include "World.h"
#include "Entity_abstracts.h"


int main()
{
	std::unordered_multimap < std::pair<int, int>, int,pair_hash > map;

	World world;
	std::pair<int, int> pos_animal1(1, 1);
	Animal* animal1 = new Animal(pos_animal1,{0,1,2,3});
	Animal* animal2 = new Animal(pos_animal1,{1,3,2,1});
	Animal* animal3 = dynamic_cast<Animal*>(animal1->bear_child(animal2));

	world.add_entity(animal1, pos_animal1);
	world.add_entity(animal2, pos_animal1);
	world.add_entity(animal3, animal3->get_position());

	world.move_entity(animal1, { 2,2 });

	try {

		std::ofstream ofs("world.txt");
		boost::archive::text_oarchive oa(ofs);
		oa << world;
		
		ofs.close();

		World world2;

		std::ifstream ifs("world.txt");
		boost::archive::text_iarchive ia(ifs);
		ia >> world2;
		
		ifs.close();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	auto i = 0;

	world.start();
}
