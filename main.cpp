#include <iostream>


#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>

#include <algorithm>

#include "playerStats.h"
#include "textlog.h"
#include "avatarRoundHP.h"
#include "Board.h"



ALLEGRO_COLOR increaseColor(ALLEGRO_COLOR color);		//TODO: volar
void DrawRoundedGradientRect(float x1, float y1, float x2, float y2, ALLEGRO_COLOR c1, ALLEGRO_COLOR c2);
void drawTexturedRectangle(int x1, int y1, int x2, int y2, ALLEGRO_BITMAP * texture, ALLEGRO_COLOR c1, double scalex, double scaleY);



using namespace std;

int main()
{

	if (!al_init() || !al_init_font_addon() || !al_init_ttf_addon() || !al_init_primitives_addon() || !al_init_image_addon())
	{
		cout << "problema inicializando allegro" << endl;
		return -1;
	}
	
	{
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

		ALLEGRO_DISPLAY * display = nullptr;
		al_set_new_display_flags(ALLEGRO_RESIZABLE);
		if (!(display = al_create_display(1200, 800)))
		{
			cout << "no se creo el display" << endl;
			return -1;
		}

		al_set_window_constraints(display, 400, 300, 0, 0);

		al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);
		ALLEGRO_BITMAP * icon = al_load_bitmap("myAvatar1.png");
		al_set_display_icon(display, icon);
		al_set_window_title(display, "Skirmish Wars");

		al_install_mouse();

		ALLEGRO_COLOR textureColor = al_map_rgb_f(1, 1, 1);
		ALLEGRO_COLOR textureColor2 = al_map_rgb_f(0.7, 1, 0.7);
		ALLEGRO_BITMAP * bgTexture = al_load_bitmap("ropeTexture.png");
		ALLEGRO_BITMAP * bgTexture2 = al_load_bitmap("cloudTexture2.png");



		textlog log(display, 0.5, 0.8, 0.5, 0.2);
		{
			log.append("Linea 1");
			log.append("Linea 2");
			log.append("Linea 3");
			log.append("Linea 4");
			log.append("Linea 5");
			log.append("Bienvenido a skirmish wars. Para iniciar, presione verde tres veces consecutivas alternadas");
			log.append("Shoot Toby twice");
			log.append("vamos cirujano yo se que usted pordra estirarla un poco mas un poco mas un poco mas");
			log.append("quinta frase mafalda es una gata malvada");
			log.append("sexta frase bluetooth bluetooth diente azul de sable blanco");
			log.append("hola, quiero mucha a mi mama que me esta ayudando a hacer esto.");
			log.append("Y tambien a mi tia gaby que me pelo la pera. Y a la tia caro que esta con mi tio adrian de vuelta. VIVA EL AMOR");
			log.append("Tinelli tinelli tinelli bienvenida como participante. no se quien baila. Es nay awada?");
			log.append("elimino el twitter?");
			log.append("kien so?");
		}
		playerStats stats(display, 0, 0.8, 0.5, 0.2);


		int avatarRadius = al_get_display_height(display) / 10;	//TODO: magic nuumber
		ALLEGRO_COLOR scoreColor = al_map_rgb(100, 200, 100);
		avatarRoundHP myHP(al_load_bitmap("png5.png"), avatarRadius, 12, 10, 10, scoreColor);
		avatarRoundHP theirHP(al_load_bitmap("armySprite.png"), avatarRadius, 12, al_get_display_width(display) - 2 * avatarRadius - 10, 10, scoreColor);



		///////////////////////////////////////////////////////////

		ALLEGRO_EVENT ev;
		ALLEGRO_EVENT_QUEUE * q;

		q = al_create_event_queue();
		al_register_event_source(q, al_get_display_event_source(display));
		al_register_event_source(q, al_get_mouse_event_source());

		unsigned int h = al_get_display_height(display) / 2;
		unsigned int w = al_get_display_width(display) / 2;
		
		Board b(display);


		ALLEGRO_COLOR lineColor = al_map_rgb(0, 0, 0);

		bool redraw = true;
		
		do
		{
			al_wait_for_event(q, &ev);
			if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
			{
				al_acknowledge_resize(display);
				log.acknowledgeResize();
				stats.acknowledgeResize();
				redraw = true;
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES && (ev.mouse.dz))
			{
				log.scroll(ev.mouse.dz);
				log.draw();
				al_flip_display();
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
			{
				redraw = true;
			}

			if (redraw)
			{
				al_clear_to_color(al_map_rgb_f(0.7, 0.7, 1));
				drawTexturedRectangle(0, 0, al_get_display_width(display), al_get_display_height(display), bgTexture2, textureColor, 1, 1);

				log.draw();

				stats.draw();

				b.draw();

				{
					theirHP.setPosition(al_get_display_width(display) - 2 * avatarRadius - 10, 10);
					al_draw_filled_rounded_rectangle(10, 10, al_get_display_width(display) - 10, avatarRadius * 2 + 10, avatarRadius, avatarRadius, al_map_rgb(0, 0, 0));
					myHP.draw();
					theirHP.draw();
					al_draw_rounded_rectangle(10, 10, al_get_display_width(display) - 10, avatarRadius * 2 + 10, avatarRadius, avatarRadius, al_map_rgb_f(0.6, 0.6, 0.6), 6);
					al_draw_rounded_rectangle(10, 10, al_get_display_width(display) - 10, avatarRadius * 2 + 10, avatarRadius, avatarRadius, al_map_rgb_f(1, 1, 1), 0);
				}

				al_flip_display();
				redraw = false;


			}

		} while (ev.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

		al_destroy_display(display);
	}

	al_shutdown_image_addon();
	al_shutdown_ttf_addon();
	al_shutdown_font_addon();
	al_shutdown_primitives_addon();

	return 0;
}



//TODO: poner offset inicial del rectangulo y a como parametros
void drawTexturedRectangle(int x1, int y1, int x2, int y2, ALLEGRO_BITMAP * texture, ALLEGRO_COLOR c1, double scaleX, double scaleY)
{
	ALLEGRO_VERTEX v[] = {
		{ x1, y1, 0, x1 / scaleX, y1 / scaleY, c1 },
		{ x2, y1, 0, x2 / scaleX, y1 / scaleY, c1 },
		{ x2, y2, 0, x2 / scaleX, y2 / scaleY, c1 },
		{ x1, y2, 0, x1 / scaleX, y2 / scaleY, c1 } };
	al_draw_prim(v, nullptr, texture, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
}




ALLEGRO_COLOR increaseColor(ALLEGRO_COLOR color)
{
	color.r += 0.01; 
	if (color.r > 1) 
		color.r -=1;

	color.g += 0.05; 
	if (color.g > 1) 
		color.g -= 1;

	color.b += 0.008; 
	if (color.b > 1) 
		color.b -= 1;

	return color;
}






///////////////////////////////////////////////////////////////////////////////
void DrawRoundedGradientRect(float x1, float y1, float x2, float y2,
	ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
	ALLEGRO_VERTEX v[4] =
	{
		{ x1, y1, 0, 0, 0, c1 },
		{ x2, y1, 0, 0, 0, c1 },
		{ x2, y2, 0, 0, 0, c2 },
		{ x1, y2, 0, 0, 0, c1 }
	};
	al_draw_prim(v, nullptr, nullptr, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
}


/********************************************************************************************/
//al_draw_prim

//void drawFunkyRectangle(int x1, int x2, int x3, int x4)
//{
//	ALLEGRO_VERTEX v[] = {
//		{256, 50, 0, 128, 0, al_map_rgb_f(1, 0, 0)},
//		{256, 256, 0, 256, 256, al_map_rgb_f(0, 0, 1)},
//		{10, 256, 0, 0, 256, al_map_rgb_f(0, 1, 0)},
//		{10, 10, 0, 256, 256, al_map_rgb_f(0, 1, 1)} };
//	al_draw_prim(v, nullptr, al_load_bitmap("paperTexture.png"), 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
//}

/********************************************************************************************/
//FILECHOOSER

//ALLEGRO_FILECHOOSER *filechooser;
//filechooser = al_create_native_file_dialog("C:", "Choose a file.", "*.*;*.png;", 1);
//al_show_native_file_dialog(display, filechooser);
//
// // Actually I will not use this but leaving it here as example only
//int counter = al_get_native_file_dialog_count(filechooser);
//
// // Instead of cycling counter, I will select 1 to refer to the first image selected
//const char* path = al_get_native_file_dialog_path(filechooser, 0);
//ALLEGRO_BITMAP *image = al_load_bitmap(path);
//al_draw_bitmap(image, 0, 0, 0);

/********************************************************************************************/



/********************************************************************************************/
//MONITOR_INFO

//ALLEGRO_MONITOR_INFO monitorInfo;
//bool go = al_get_monitor_info(0, &monitorInfo);
//
//al_reset_new_display_options();
//
//al_set_new_display_option(ALLEGRO_UPDATE_DISPLAY_REGION, true, ALLEGRO_REQUIRE);
//al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, true, ALLEGRO_REQUIRE);
//
//ALLEGRO_DISPLAY * display = al_create_display(monitorInfo.x2 / 2, monitorInfo.y2 / 2);
//
//int optionUpdate = al_get_display_option(display, ALLEGRO_UPDATE_DISPLAY_REGION);
//
//al_clear_to_color(al_map_rgb(70, 150, 0));
//al_update_display_region(0, 0, 100, 100);
//al_rest(2.0);
//
//al_update_display_region(0, 0, 100, 200);
//
//getchar();

/********************************************************************************************/


/********************************************************************************************/
//DISPLAY_RESIZE

//ALLEGRO_BITMAP * img = nullptr;
//if (!(img = al_load_bitmap("png4.png")))
//{
//	cout << "no se cargo la imagen" << endl;
//	//shutdownAllegro();
//	return -1;
//}
//
//
//ALLEGRO_EVENT_QUEUE * q;
//if (!(q = al_create_event_queue()))
//{
//	cout << "no se creo la cola de eventos" << endl;
//	//shutdownAllegro();
//	return -1;
//}
//
//al_register_event_source(q, al_get_display_event_source(display));
//
//ALLEGRO_EVENT ev;
//
//do
//{
//	al_draw_scaled_bitmap(img,
//		0, 0, al_get_bitmap_width(img), al_get_bitmap_height(img),
//		0, 0, al_get_display_width(display), al_get_display_height(display), 0);
//	al_flip_display();
//
//	al_wait_for_event(q, &ev);
//	if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
//	{
//		al_acknowledge_resize(display);
//		cout << "resize\n";
//	}
//} while (ev.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

/********************************************************************************************/


/********************************************************************************************/
//CROP TO CIRCLE WITH BLENDER


//ALLEGRO_BITMAP * interestingbmp = al_load_bitmap("png2.png");
//int wi = al_get_bitmap_width(interestingbmp), he = al_get_bitmap_height(interestingbmp);
//
//int op, src, dst, aop, asrc, adst;
//al_get_separate_blender(&op, &src, &dst, &aop, &asrc, &adst);
//
//ALLEGRO_BITMAP * clearbmp = al_create_bitmap(wi, he);
//al_set_target_bitmap(clearbmp);
//al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
//al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);	//sirve para que se mantenga la transparencia del circulo dibujado abajo
//al_draw_filled_circle(wi / 2, he / 2, min(wi, he) / 2, al_map_rgba_f(0, 0, 0, 0));
//
////setear blender para que solo se mantenga la parte de intereseting bmp que se va a dibujar sobre el circulo. lo demas es transparente.
//al_set_blender(ALLEGRO_SRC_MINUS_DEST, ALLEGRO_ONE, ALLEGRO_ONE);
//al_draw_bitmap(interestingbmp, 0, 0, 0);
//
//al_set_separate_blender(op, src, dst, aop, asrc, adst);	//volver al blender inicial
//al_set_target_backbuffer(display);	//resetear el backbuffer porque antes estaba en clearbmp
//al_draw_bitmap(clearbmp, 0, 0, 0);	//se va a ver solamente una seccion circular de interestingbmp
//al_flip_display();

/********************************************************************************************/