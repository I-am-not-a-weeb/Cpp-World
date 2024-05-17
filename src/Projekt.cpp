#pragma warning(disable: 4996)
#include <iostream>
#include <conio.h>
#include <unordered_map>
#include <utility>

#include <boost/container_hash/hash.hpp>

#include <pplx/pplxtasks.h>

#include "include/pair_hash.h"

#include "include/World.h"
#include "include/Abstract_entities.h"
#include "include/Concrete_entities.h"
#include <concurrent_unordered_map.h>

#include <random>
#include <Windows.h>



#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

std::string world_filename{ "world.txt" };

int main()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	int offset_x = 0;
	int offset_y = 0;

	bool is_running = true;

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cerr.tie(nullptr);

	World world;
	std::pair<int, int> pos_animal1(1, 1);
	/*Animal* animal1 = new Animal(&world,pos_animal1,{0,1,2,3});
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
	*/

	Sheep* sheep1 = new Sheep(&world, pos_animal1);
	Sheep* sheep2 = new Sheep(&world, pos_animal1);	
	Sheep* sheep3 = dynamic_cast<Sheep*>(sheep1->bear_child(sheep2));
	Sheep* sheep4 = dynamic_cast<Sheep*>(sheep2->bear_child(sheep3));
	Sheep* sheep5 = dynamic_cast<Sheep*>(sheep3->bear_child(sheep1));

	std::pair<int, int> pos_animal2(3, 3);

	Fox* fox1 = new Fox(&world, pos_animal2);
	Fox* fox2 = new Fox(&world, pos_animal2);
	Fox* fox3 = dynamic_cast<Fox*>(fox1->bear_child(fox2));
	Fox* fox4 = dynamic_cast<Fox*>(fox2->bear_child(fox3));
	Fox* fox5 = dynamic_cast<Fox*>(fox3->bear_child(fox1));

	world.add_entity(sheep1, pos_animal1);
	world.add_entity(sheep2, pos_animal1);
	world.add_entity(sheep3, sheep3->get_position());
	world.add_entity(sheep4, sheep4->get_position());
	world.add_entity(sheep5, sheep5->get_position());

	world.add_entity(fox1, pos_animal2);
	world.add_entity(fox2, pos_animal2);
	world.add_entity(fox3, fox3->get_position());
	world.add_entity(fox4, fox4->get_position());
	world.add_entity(fox5, fox5->get_position());

	std::wstring com;

	char* last_action = new char[40];
	world.set_last_action(last_action);

	auto* speed = new unsigned int;
	*speed = 100;

	world.set_speed(speed);

	auto main = pplx::create_task([&world,&com,&csbi,&is_running,&offset_y,&offset_x,&speed] {

		while(is_running)
		{
			char click = _getch();

			//printf("%d", click);

			switch (click)
			{
			case KEY_UP:
				{
				offset_y--;
				break;
				}
			case KEY_DOWN:
				{
					offset_y++;
					break;
				}

			case KEY_LEFT:
				{
					offset_x--;
					break;
				}

			case KEY_RIGHT:
				{
					offset_x++;
					break;
				}
			case 115:	// s
				{
					world.save(world_filename);
					break;
				}
			case 108:	// l
				{
					world.load_this(world_filename);
					break;
				}

			case 119:	// w
				{
					world.start();
					break;
				}

			case  112:	// p
				{
					world.stop();
					break;
				}
			case 101:	// e
				{
					is_running = false;
					world.stop().wait();
					break;
				}
			
			case 105:	// i	
				{
					if(*speed >= 10)
						*speed += 10;
					else if(*speed >= 0)
						*speed += 1;
					break;
				}
			case 107:	// k
				{
					if(*speed > 10)
						*speed -= 10;
					else if(*speed > 0)
						*speed -= 1;
					break;
				}
			}

			/*std::wcin >> com;
			if (com == L"exit")
			{
				is_running = false;
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
			else if(com == L"print")
			{
				world.print();
			}
			else if(com == L"clear")
			{
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

				int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
				int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

				//std::cout << "aaaaabaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaadaaaaaaaaaaaaa\r";
				//std::cout << "\x1b[1F" << "\x1b[1F" << "\x1b[1F";
				std::cout << "\n " << columns << " " << rows << "\r";
				std::cout << "\x1B[2J\x1B[H";
			}*/
		}
	});

	while (is_running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

		unsigned int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		unsigned int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		if(rows - 5 < 0) continue;

		char* screen = new char[columns * (rows)+1];
		memset(screen, ' ', columns * (rows));
		
		//std::cout << "\x1b[1F" << "\x1b[2K";
		screen[columns * (rows)] = '\0';

		//printf("%d", strlen(screen));

		for (unsigned int i{ 0 }; i < rows - 5; i++)
		{
			unsigned int y_with_offset{ offset_y + i };

			for (unsigned int j{ 0 }; j < columns; j++)
			{
				unsigned int x_with_offset{ offset_x + j };

				world.world_map_mutex.lock();
				world.get_entity({ x_with_offset,y_with_offset }) == nullptr ? 0 : screen[i*columns + j] = world.get_entity({ x_with_offset,y_with_offset })->ident();
				world.world_map_mutex.unlock();
			}
		}
		char*buffer = new char[columns];
		sprintf(buffer, "offset x: %d, offset y: %d Speed: %d", offset_x, offset_y,*speed);
		strncpy(screen + columns * (rows - 5), buffer,strlen(buffer));

		sprintf(buffer, "w - start, p - pause, e - exit, s - save, l - load");
		strncpy(screen + columns * (rows - 3), buffer, strlen(buffer));

		sprintf(buffer, "  arrows - move, i - faster, k - slower");
		strncpy(screen + columns * (rows - 2), buffer, strlen(buffer));


		sprintf(buffer, "Last action: %s",last_action);
		strncpy(screen + columns * (rows - 1), buffer, strlen(buffer));

		//std::cout << "\x1b[1F";
		printf("%s%s", "\x1B[2J\x1B[H\0", screen);
		//std::cout << *screen;

	}


	main.wait();
}
