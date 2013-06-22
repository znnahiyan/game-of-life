#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

#define DEAD false
#define ALIVE true

typedef struct
{
	bool state;
	uint8_t neighbours;
} cell_t;
	
int main()
{
	// Variable declaration! --
	sf::RenderWindow window(sf::VideoMode(800, 600), "Wazzaaaa");
	
	bool running = true;
	sf::Event event;
	
	sf::Vector2i mouse_pos;
	bool mouse_down;
	
	// scale is the pixel-to-cellsize scale; bounds will be set to world size
	sf::Vector2u scale(8, 8), bounds(window.getSize()); 
	cell_t **cells;
	
	sf::RectangleShape live_cell(sf::Vector2f(8, 8)), dead_cell(sf::Vector2f(8, 8));
	
	// colour cell shapes
	dead_cell.setFillColor(sf::Color(0x00, 0x00, 0x00));
	live_cell.setFillColor(sf::Color(0xFF, 0xFF, 0xFF));
	
	// seed random() for initialisation
	srandom( time(0) );
	
	// adjust according to the pixel-to-cellsize scale
	bounds.x /= scale.x;
	bounds.y /= scale.y;
	
	// allocate and initialise the cells
	cells = new cell_t* [bounds.x];
	
	for (sf::Vector2u slot(0, 0); slot.x < bounds.x; ++slot.x)
	{
		cells[slot.x] = new cell_t [bounds.y];
		
		for (slot.y = 0; slot.y < bounds.y; ++slot.y)
			cells[slot.x][slot.y] = cell_t{bool (random() & 0x1), 0};
	}
	
	// main loop
	while (running && window.isOpen())
	{
		// Handle events --
		while (window.pollEvent(event))
			switch (event.type)
			{
				case sf::Event::Closed:
					running = false;
				
				// Mouse handlers!
				case sf::Event::MouseButtonPressed:
					mouse_down = true;
				
				case sf::Event::MouseButtonReleased:
					mouse_down = false;
					
			}
		
		// Mouse actions! New life and death!
		mouse_pos = sf::Mouse::getPosition(window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left )) cells[mouse_pos.x / scale.x][mouse_pos.y / scale.y].state = ALIVE;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) cells[mouse_pos.x / scale.x][mouse_pos.y / scale.y].state = DEAD;

		// Calculate life! --
		
		// Check for thee edge cases and update thy neighbours!
		for (sf::Vector2u slot(0, 0); slot.x < bounds.x; ++slot.x)
			for (slot.y = 0; slot.y < bounds.y; ++slot.y)
				if (cells[slot.x][slot.y].state == ALIVE)
				{
					if (slot.x != 0            && slot.y != 0) cells[slot.x - 1][slot.y - 1].neighbours++;
					if (                          slot.y != 0) cells[slot.x    ][slot.y - 1].neighbours++;
					if (slot.x != bounds.x - 1 && slot.y != 0) cells[slot.x + 1][slot.y - 1].neighbours++;
					
					if (slot.x != 0           ) cells[slot.x - 1][slot.y    ].neighbours++;
					if (slot.x != bounds.x - 1) cells[slot.x + 1][slot.y    ].neighbours++;
					
					if (slot.x != 0            && slot.y != bounds.y - 1) cells[slot.x - 1][slot.y + 1].neighbours++;
					if (                          slot.y != bounds.y - 1) cells[slot.x    ][slot.y + 1].neighbours++;
					if (slot.x != bounds.x - 1 && slot.y != bounds.y - 1) cells[slot.x + 1][slot.y + 1].neighbours++;
				}
		
		// Now update the states..
		for (sf::Vector2u slot(0, 0); slot.x < bounds.x; ++slot.x)
			for (slot.y = 0; slot.y < bounds.y; ++slot.y)
			{
				// cell would die when n < 2; undercrowding/population
				if (cells[slot.x][slot.y].neighbours < 2)
					cells[slot.x][slot.y].state = DEAD;
				
				// do not account for when n == 2, if cell was previously alive, it would stay keep it alive; if cell was previously dead, keep dead
				// no change would happen so there's no need to account for it.
				
				// cell would always be[come] alive when n == 3
				if (cells[slot.x][slot.y].neighbours == 3)
					cells[slot.x][slot.y].state = ALIVE;
				
				// death by overpopulation ftw
				if (cells[slot.x][slot.y].neighbours > 3)
					cells[slot.x][slot.y].state = DEAD;
					
				// while we're here, let's reset the neighbours count for later!
				cells[slot.x][slot.y].neighbours = 0;
			}
		
		// RENDER! --
		window.clear(sf::Color::Black); // Clear it!
		
		dead_cell.setPosition(0, 0);
		live_cell.setPosition(0, 0);
		
		for (sf::Vector2u slot(0, 0); slot.x < bounds.x; ++slot.x)
		{
			dead_cell.setPosition(dead_cell.getPosition().x + scale.x, 0);
			live_cell.setPosition(live_cell.getPosition().x + scale.x, 0);
			
			for (slot.y = 0; slot.y < bounds.y; ++slot.y)
			{
				if (cells[slot.x][slot.y].state == ALIVE)
					window.draw(live_cell);
				else
					window.draw(dead_cell);
				
				dead_cell.move(0, scale.y);
				live_cell.move(0, scale.y);
			}	
		}
		
		window.display(); // DISPLAY!
	}
	
	
	return 0;
}
