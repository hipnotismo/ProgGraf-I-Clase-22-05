#include <iostream>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

struct MovingObject
{
	ALLEGRO_BITMAP *image;
	int width;
	int height;
	int x;
	int y;
};

int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *display = NULL;

	int window_width = 800;
	int window_height = 600;

	display = al_create_display(window_width, window_height);

	if (!al_init())
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if (!al_init_image_addon())
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	if (!display)
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	al_install_keyboard();

	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	// Player
	MovingObject player;
	player.image = al_load_bitmap("AliensShip.png");
	player.x = 25;
	player.y = 25;
	player.width = 32;
	player.height = 32;

	// Coin
	MovingObject coin;
	coin.image = al_load_bitmap("Coin.png");
	coin.x = window_height - 100;
	coin.y = window_width - 100;
	coin.width = 24;
	coin.height = 24;

	// Variables
	bool playing = true;
	bool win = false;
	int movSpeed = 3;

	// First draw
	al_draw_bitmap(player.image, player.x, player.y, 0);
	al_draw_bitmap(coin.image, coin.x, coin.y, 0);

	while (playing)
	{
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		// Check movement
		if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				if (player.y > movSpeed)
					player.y -= movSpeed;
				break;
			case ALLEGRO_KEY_DOWN:
				if (player.y < window_height - movSpeed - player.height)
					player.y += movSpeed;
				break;
			case ALLEGRO_KEY_RIGHT:
				if (player.x < window_width - movSpeed - player.width)
					player.x += movSpeed;
				break;
			case ALLEGRO_KEY_LEFT:
				if (player.x > movSpeed)
					player.x -= movSpeed;
				break;
			case ALLEGRO_KEY_ESCAPE:
				playing = false;
				break;
			}

			// Collision Player-Coin
			if ((player.x >= coin.x				    &&  player.x <= coin.x + coin.width) ||
				(player.x + player.width >= coin.x  &&  player.x + player.width <= coin.x + coin.width))
			{
				if ((player.y >= coin.y				     &&  player.y <= coin.y + coin.height) ||
					(player.y + player.height >= coin.y  &&  player.y + player.height <= coin.y + coin.height))
				{
					win = true;
					playing = false;
				}
			}
		}

		// Screen update
		al_draw_bitmap(player.image, player.x, player.y, 0);
		al_draw_bitmap(coin.image, coin.x, coin.y, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

	// "Destructor"
	al_destroy_display(display);
	al_destroy_bitmap(player.image);
	al_destroy_bitmap(coin.image);

	return 0;
}