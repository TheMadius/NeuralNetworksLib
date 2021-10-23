#include "ModelView.h"

ModelView::ModelView()
{
	game = new WindowGame;
	game->app = new (RenderWindow)(VideoMode(1024, 768), "SFML Checkers!", Style::Close | sf::Style::Titlebar);
	LoadTexturesAndText();
}

ModelView::ModelView(std::string name, int windowHight, int windowWidth)
{
	game = new WindowGame;
	game->app = new (RenderWindow)(VideoMode(windowHight, windowWidth), name, Style::Close | sf::Style::Titlebar);
	LoadTexturesAndText();
}

void ModelView::LoadTexturesAndText()
{
	//спрайты для игровой доски
	game->gameTextureAndText.whiteSprite = new Sprite[FILD_SIZE * (FILD_SIZE / 2)];
	game->gameTextureAndText.blueSprite = new Sprite[FILD_SIZE * (FILD_SIZE / 2)];
	game->gameTextureAndText.greenSprite = new Sprite[FILD_SIZE * FILD_SIZE];

	//спрайты для шашек
	game->gameTextureAndText.blackСheckerSprite = new Sprite[COUNT_CHECKER];
	game->gameTextureAndText.whiteСheckerSprite = new Sprite[COUNT_CHECKER];

	game->gameTextureAndText.font.loadFromFile("..\\Debug\\fonts\\CyrilicOld.TTF");
	game->gameTextureAndText.white.loadFromFile("..\\Debug\\textures\\White.png");
	game->gameTextureAndText.blue.loadFromFile("..\\Debug\\textures\\Blue.png");
	game->gameTextureAndText.green.loadFromFile("..\\Debug\\textures\\Green.JPG");
	game->gameTextureAndText.blackСhecker.loadFromFile("..\\Debug\\textures\\blackCh.png");
	game->gameTextureAndText.whiteСhecker.loadFromFile("..\\Debug\\textures\\whiteCh.png");

	for (size_t i = 0; i < (game->gameBoard.size * (game->gameBoard.size / 2)); i++)
	{
		game->gameTextureAndText.whiteSprite[i].setTexture(game->gameTextureAndText.white);
		game->gameTextureAndText.blueSprite[i].setTexture(game->gameTextureAndText.blue);
	}
	//	greenSprite->setTexture(green);
	//
	for (size_t i = 0; i < COUNT_CHECKER; i++)
	{
		game->gameTextureAndText.blackСheckerSprite[i].setTexture(game->gameTextureAndText.blackСhecker);
		game->gameTextureAndText.whiteСheckerSprite[i].setTexture(game->gameTextureAndText.whiteСhecker);
	}

	//Текст
	game->gameTextureAndText.txt = new Text;
	game->gameTextureAndText.txt->setFont(game->gameTextureAndText.font);
	game->gameTextureAndText.txt->setCharacterSize(FONT_SIZE);
	game->gameTextureAndText.txt->setFillColor(TEXT_COLOR);
	game->gameTextureAndText.txt->setStyle(TEXT_TYPE);
}

void ModelView::Start()
{
	GameBoardInit();

	while (game->app->isOpen())
	{
		game->app->clear(Color::Black);
		Events();
		GameBoardUpdate();
		game->app->display();
	}
}

void ModelView::Events()
{
	while (game->app->pollEvent(game->e))
	{
		if (game->e.type == Event::Closed)
			game->app->close();

		if (game->e.type == Event::MouseButtonPressed)//если нажата клавиша мыши
			if (game->e.key.code == Mouse::Left) {//левая
				SelectСheckers();
			}
	}
}


void ModelView::GameBoardInit()
{
	//Заполнить графическое поле значениями
	for (int i = 0; i < FILD_SIZE; i++)
		for (int j = 0; j < FILD_SIZE; j++)
		{
			if (i % 2 == 0)
			{
				game->gameBoard.gridView[i][j] = (j % 2 == 0) ? White : Blue;
			}
			else
			{
				game->gameBoard.gridView[i][j] = (j % 2 == 0) ? Blue : White;
			}
		}

	//Заполнить логическое поле значениями
	for (int i = 0; i < FILD_SIZE; i++)
		for (int j = 0; j < FILD_SIZE; j++)
		{
			if (i % 2 != 0)
			{
				if (j < FILD_SIZE - 5)
					game->gameBoard.gridLogic[i][j] = (j % 2 == 0) ? WhiteChecker : EmptyChecker;
				if (j > FILD_SIZE - 4)
					game->gameBoard.gridLogic[i][j] = (j % 2 == 0) ? BlackChecker : EmptyChecker;
				if ((j == 3 || j == 4))
					game->gameBoard.gridLogic[i][j] = EmptyChecker;
			}
			else
			{
				if (j < FILD_SIZE - 5)
					game->gameBoard.gridLogic[i][j] = (j % 2 == 0) ? EmptyChecker : WhiteChecker;
				if (j > FILD_SIZE - 4)
					game->gameBoard.gridLogic[i][j] = (j % 2 == 0) ? EmptyChecker : BlackChecker;
				if ((j == 3 || j == 4))
					game->gameBoard.gridLogic[i][j] = EmptyChecker;
			}
		}

}

void ModelView::GameBoardUpdate()
{
	//Создаем белый прямоугольник (главная подложка доски) 
	RectangleShape rectangleBoardWhite(Vector2f((game->gameBoard.cellSize * (game->gameBoard.size + 1)), (game->gameBoard.cellSize * (game->gameBoard.size + 1))));
	//Перемещаем его в нижний ряд справа от многоугольника
	rectangleBoardWhite.move((game->gameBoard.cellSize / 2), (game->gameBoard.cellSize / 2));
	//Устанавливаем ему цвет
	rectangleBoardWhite.setFillColor(WHITE);
	//Отрисовка прямоугольника
	game->app->draw(rectangleBoardWhite);

	//Создаем чёрный прямоугольник (заполнение промежутков между ячейками) (5 и 10 зависимы и отвечают за размер рамки)
	RectangleShape rectangleBoardBlack(Vector2f(game->gameBoard.cellSize * game->gameBoard.size + 10, game->gameBoard.cellSize * game->gameBoard.size + 10));
	//Перемещаем его в нижний ряд справа от многоугольника
	rectangleBoardBlack.move(game->gameBoard.cellSize - 5, game->gameBoard.cellSize - 5);
	//Устанавливаем ему цвет
	rectangleBoardBlack.setFillColor(BLACK);
	//Отрисовка прямоугольника
	game->app->draw(rectangleBoardBlack);

	//Отрисовка букв и цифр на поле
	//координаты для букв
	int digitStartPosLeftX = game->gameBoard.cellSize - game->gameBoard.cellSize / 3;
	int digitStartPosRightX = (game->gameBoard.cellSize * 9) + game->gameBoard.cellSize / 6;
	//координаты для букв
	int charPosX = game->gameBoard.cellSize + game->gameBoard.cellSize / 3;
	int	charStartPosTopY = game->gameBoard.cellSize / 2;
	int	charStartPosBottomY = (game->gameBoard.cellSize * 9);

	for (int i = 1, digitPosY = (game->gameBoard.cellSize + (game->gameBoard.cellSize / 5)); i < 9; i += 1, digitPosY += game->gameBoard.cellSize)
	{
		game->gameTextureAndText.txt->setPosition(digitStartPosLeftX, digitPosY);
		game->gameTextureAndText.txt->setString(std::to_string(i));
		game->app->draw(*game->gameTextureAndText.txt);

		game->gameTextureAndText.txt->setPosition(digitStartPosRightX, digitPosY);
		game->gameTextureAndText.txt->setString(std::to_string(i));
		game->app->draw(*game->gameTextureAndText.txt);
	}

	for (char ch = 'a'; ch < 'i'; ch += 1, charPosX += game->gameBoard.cellSize)
	{
		game->gameTextureAndText.txt->setPosition(charPosX, charStartPosTopY);
		game->gameTextureAndText.txt->setString(ch);
		game->app->draw(*game->gameTextureAndText.txt);

		game->gameTextureAndText.txt->setPosition(charPosX, charStartPosBottomY);
		game->gameTextureAndText.txt->setString(ch);
		game->app->draw(*game->gameTextureAndText.txt);
	}

	//рисование определённых квадратов на поле и шашек
	int whiteSpriteIndex = 0;
	//int greenSpriteIndex = 0;
	int blueSpriteIndex = 0;
	int whiteСheckerSpriteIndex = 0;
	int blackСheckerSpriteIndex = 0;

	for (int i = 0; i < FILD_SIZE; i++)
		for (int j = 0; j < FILD_SIZE; j++)
		{
			if (game->gameBoard.gridView[i][j] == White)
			{
				// Устанавливаем его в заданную позицию...
				game->gameTextureAndText.whiteSprite[whiteSpriteIndex].setPosition((i + 1) * game->gameBoard.cellSize, (j + 1) * game->gameBoard.cellSize);
				//	Отрисовывание
				game->app->draw(game->gameTextureAndText.whiteSprite[whiteSpriteIndex]);
				whiteSpriteIndex++;
			}
			//			else if (game->gameBoard.gridView[i][j] == Green) {
			//				// Устанавливаем его в заданную позицию
			//				game->gameTextureAndText.greenSprite[greenSpriteIndex].setPosition((i + 1) * game->gameBoard.cellSize, (j + 1) * game->gameBoard.cellSize);
			//				// Отрисовывание
			//				game->app->draw(greenSprite[greenSpriteIndex]);
			//				greenSpriteIndex++;
			//			}
			else {
				// Устанавливаем его в заданную позицию
				game->gameTextureAndText.blueSprite[blueSpriteIndex].setPosition((i + 1) * game->gameBoard.cellSize, (j + 1) * game->gameBoard.cellSize);
				// Отрисовывание
				game->app->draw(game->gameTextureAndText.blueSprite[blueSpriteIndex]);
				blueSpriteIndex++;
			}

			//рисование шашек на поле
			if (game->gameBoard.gridLogic[i][j] == WhiteChecker)
			{
				// Устанавливаем его в заданную позицию...
				game->gameTextureAndText.whiteСheckerSprite[whiteСheckerSpriteIndex].setPosition((i + 1) * game->gameBoard.cellSize + 7, (j + 1) * game->gameBoard.cellSize + 7);
				//	Отрисовывание
				game->app->draw(game->gameTextureAndText.whiteСheckerSprite[whiteСheckerSpriteIndex]);
				whiteСheckerSpriteIndex++;
			}
			else if (game->gameBoard.gridLogic[i][j] == BlackChecker) {
				// Устанавливаем его в заданную позицию
				game->gameTextureAndText.blackСheckerSprite[blackСheckerSpriteIndex].setPosition((i + 1) * game->gameBoard.cellSize + 7, (j + 1) * game->gameBoard.cellSize + 7);
				// Отрисовывание
				game->app->draw(game->gameTextureAndText.blackСheckerSprite[blackСheckerSpriteIndex]);
				blackСheckerSpriteIndex++;
			}
		}
}

void ModelView::SelectСheckers()
{
	for (size_t i = 0; i < FILD_SIZE * FILD_SIZE; i++)
	{
		if ((i < 32) && (game->gameTextureAndText.whiteSprite[i % 32].getGlobalBounds().contains(game->e.mouseButton.x, game->e.mouseButton.y)))
		{
			std::cout << "White: " << i + 1 << std::endl;
			std::cout << "mouse x: " << game->e.mouseButton.x << std::endl;
			std::cout << "mouse y: " << game->e.mouseButton.y << "\n" << std::endl;
		}
		if ((i < 32) && (game->gameTextureAndText.blueSprite[i % 32].getGlobalBounds().contains(game->e.mouseButton.x, game->e.mouseButton.y)))
		{
			std::cout << "Blue: " << i + 1 << std::endl;
			std::cout << "mouse x: " << game->e.mouseButton.x << std::endl;
			std::cout << "mouse y: " << game->e.mouseButton.y << "\n" << std::endl;
		}
		if ((i < 12) && (game->gameTextureAndText.whiteСheckerSprite[i % 12].getGlobalBounds().contains(game->e.mouseButton.x, game->e.mouseButton.y)))
		{

			std::cout << "White Checker Sprite: " << i + 1 << std::endl;
			std::cout << "mouse x: " << game->e.mouseButton.x << std::endl;
			std::cout << "mouse y: " << game->e.mouseButton.y << "\n" << std::endl;
		}
		if ((i < 12) && (game->gameTextureAndText.blackСheckerSprite[i % 12].getGlobalBounds().contains(game->e.mouseButton.x, game->e.mouseButton.y)))
		{

			std::cout << "Black Checker Sprite: " << i + 1 << std::endl;
			std::cout << "mouse x: " << game->e.mouseButton.x << std::endl;
			std::cout << "mouse y: " << game->e.mouseButton.y << "\n" << std::endl;

		}
	}
}