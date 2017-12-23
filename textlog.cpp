#include "textlog.h"
#include <iostream>

using namespace std;


bool multilineCB(int line_num, const char *line, int size, void *usefulInfo);

struct cbInfo_t {
	vector<string> * formattedMsg;
	unsigned int maxLines;
};


textlog::textlog(ALLEGRO_DISPLAY * display, float startX, float startY, float width, float height) 
	: contentBox(display, startX, startY, width, height)
{
	if (!(font = al_load_ttf_font("runescape_chat_font.ttf", 20, 0)))
	{
		cout << "font no se cargo" << endl;		//TODO: control de error
	}

	lineHeight = al_get_font_line_height(font);
	maxLinesDisplayed = contentHeight / lineHeight;
	totalLines = 0;
	scrollOffset = 0;
	fontColor = al_map_rgb_f(1, 1, 1);		//TODO: sacar magic color
}

textlog::~textlog()
{
	al_destroy_font(font);
	clear();
}

void textlog::append(std::string newMsg)
{
	newMsg = " >> " + newMsg;	//TODO: modularizar el "line header".

	vector<string> formattedMsg;

	cbInfo_t usefulInfo = { &formattedMsg, maxLinesDisplayed };
	al_do_multiline_text(font,			//TODO: esto se repite en otro lado, unir en una sola funcion
		contentWidth,
		newMsg.c_str(),
		multilineCB,
		(void *)&usefulInfo);

	log.push_back(formattedMsg);
	totalLines += formattedMsg.size();
}

void textlog::append(char * newMsg)
{
	append(string(newMsg));
}

void textlog::scroll(signed int offset)
{
	scrollOffset += offset;	//TODO: chequear bien para overflow y underflow. como limito prolijamente???
	if (scrollOffset < 0)
	{
		scrollOffset = 0;
	}
	else if (scrollOffset > totalLines - maxLinesDisplayed)
	{
		scrollOffset = totalLines - maxLinesDisplayed;
	}
	drawContent();		//TODO: drawcontent o drawBox???
}

void textlog::clear()
{
	log.clear();
}


void textlog::drawContent()
{
	unsigned int analysedLines = 0;

	for (list<vector<string>>::reverse_iterator currentMsg = log.rbegin(); //recorre los mensajes dentro de la lista y los dibuja
		currentMsg != log.rend() && analysedLines <= (maxLinesDisplayed + scrollOffset);	//hasta que se terminen los mensajes o no haya mas lugar para dibujar mas lineas
		++currentMsg)	//dibuja un mensaje por iteracion
						//TODO: se podra acortar este for? es medio feo
	{
		for (int i = 0;	//recorre las lineas dentro de un mensaje y las dibuja
			analysedLines <= (maxLinesDisplayed + scrollOffset) && i < (*currentMsg).size();	//hasta que se terminen las lineas o no haya mas lugar para dibujarlas
			++i, ++analysedLines)	//dibuja una linea por iteracion
		{
			if (analysedLines >= scrollOffset)
			{
				al_draw_text(font, fontColor,
					contentStartX, contentStartY + contentHeight - (analysedLines - scrollOffset + 1) * lineHeight,
					ALLEGRO_ALIGN_LEFT,
					(*currentMsg)[(*currentMsg).size() - 1 - i].c_str());	//TODO: algo, es horrible.
				
			}
		}
	}

	//unsigned int scrolledLines = 0;
	//lineOffset = 1;
	//unsigned int drawnLines = 0;
	//unsigned int i;	//contador
	//list<vector<string>>::reverse_iterator currentMsg;

	//for (currentMsg = log.rbegin(); //recorre los mensajes dentro de la lista y los dibuja
	//	 currentMsg != log.rend() && scrolledLines <= lineOffset;	//hasta que se terminen los mensajes o no haya mas lugar para dibujar mas lineas
	//	 ++currentMsg)	//dibuja un mensaje por iteracion
	//					//TODO: se podra acortar este for? es medio feo
	//{

	//	for (i = 0;	//recorre las lineas dentro de un mensaje y las dibuja
	//		scrolledLines <= lineOffset && i < (*currentMsg).size();	//hasta que se terminen las lineas o no haya mas lugar para dibujarlas
	//		++i, ++scrolledLines)	//dibuja una linea por iteracion
	//	{
	//		al_draw_text(font, al_map_rgb(255,0,0),
	//			contentStartX, contentStartY + contentHeight - (drawnLines + 1) * lineHeight,
	//			ALLEGRO_ALIGN_LEFT,
	//			(*currentMsg)[(*currentMsg).size() - 1 - i].c_str());	//TODO: algo, es horrible.
	//	}
	//}

	//al_flip_display();

	//for (;//list<vector<string>>::reverse_iterator currentMsg = log.rbegin(); //recorre los mensajes dentro de la lista y los dibuja
	//	currentMsg != log.rend() && drawnLines <= maxLines;	//hasta que se terminen los mensajes o no haya mas lugar para dibujar mas lineas
	//	++currentMsg)	//dibuja un mensaje por iteracion
	//	//TODO: se podra acortar este for? es medio feo
	//{
	//	cout << "distancia a mensaje final = " << distance(currentMsg, log.rend()) << endl;

	//	for (;	//recorre las lineas dentro de un mensaje y las dibuja
	//		drawnLines <= maxLines && i < (*currentMsg).size();	//hasta que se terminen las lineas o no haya mas lugar para dibujarlas
	//		++i, ++drawnLines)	//dibuja una linea por iteracion
	//	{
	//		al_draw_text(font, fontColor,
	//			contentStartX, contentStartY + contentHeight - (drawnLines + 1) * lineHeight,
	//			ALLEGRO_ALIGN_LEFT,
	//			(*currentMsg)[(*currentMsg).size() - 1 - i].c_str());	//TODO: algo, es horrible.
	//	}
	//}
}

void textlog::resizeContent()
{
	list<vector<string>> newLog;

	for (list<vector<string>>::iterator currentMsg = log.begin(); currentMsg != log.end(); ++currentMsg)
	{
		vector<string> formattedMsg;
		string msg("");

		unsigned int lines = (*currentMsg).size();

		for (unsigned int i = 0; i < lines; ++i)
		{
			msg += (*currentMsg)[i];
			if (i + 1 < lines)
			{
				msg += " ";
			}
		}

		cbInfo_t usefulInfo = { &formattedMsg, maxLinesDisplayed };
		al_do_multiline_text(font,
			contentWidth,
			msg.c_str(),
			multilineCB,
			(void *)&usefulInfo);

		newLog.push_back(formattedMsg);
	}
	log = newLog;

	maxLinesDisplayed = contentHeight / lineHeight;
}


bool multilineCB(int line_num, const char * line, int size, void * usefulInfo)
{
	cbInfo_t * info = (cbInfo_t *)usefulInfo;
	char * newLine = strncpy(new char[size + 1], line, size);		//TODO: el arreglo creado se destruye cuando se hace pop_back() o cuando se destruye la lista??
	newLine[size] = '\0';											//TODO: aparentemente strncpy es una verga, que usamos? strlcpy seria la opcion que tira google pero no es estandar o no se, no entiendo nada
	info->formattedMsg->push_back(string(newLine));

	return true;
}

