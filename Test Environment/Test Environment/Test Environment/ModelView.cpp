#include "ModelView.h"
#include "Log.h"

using namespace std;

ModelView::ModelView()
{
	windowGame = new WindowGame;
	windowGame->app = new RenderWindow(VideoMode(1024, 768), "SFML Checkers!", Style::Close | sf::Style::Titlebar);
	Log::Write(to_string((int)windowGame->app));
	LoadTexturesAndText();
}

ModelView::ModelView(std::string name, int windowHight, int windowWidth)
{
	windowGame = new WindowGame;
	windowGame->app = new RenderWindow(VideoMode(windowHight, windowWidth), name, Style::Close | sf::Style::Titlebar);
	Log::Write(to_string((int)windowGame->app));
	LoadTexturesAndText();
}

void ModelView::LoadTexturesAndText()
{
	windowGame->gameTextureAndText.font.loadFromFile("fonts\\CyrilicOld.TTF");

	windowGame->gameTextureAndText.white.loadFromFile("textures\\White.png");
	windowGame->gameTextureAndText.blue.loadFromFile("textures\\Blue.png");
	windowGame->gameTextureAndText.green.loadFromFile("textures\\Green.JPG");
	windowGame->gameTextureAndText.yellow.loadFromFile("textures\\Yellow.png");

	windowGame->gameTextureAndText.blackСhecker.loadFromFile("textures\\blackCh.png");
	windowGame->gameTextureAndText.whiteСhecker.loadFromFile("textures\\whiteCh.png");
	windowGame->gameTextureAndText.blackСheckerDM.loadFromFile("textures\\blackChDM.png");
	windowGame->gameTextureAndText.whiteСheckerDM.loadFromFile("textures\\whiteChDN.png");

	windowGame->gameTextureAndText.newGameButton.loadFromFile("textures\\NewGame.png");


	windowGame->gameTextureAndText.whiteSprite.setTexture(windowGame->gameTextureAndText.white);
	windowGame->gameTextureAndText.blueSprite.setTexture(windowGame->gameTextureAndText.blue);
	windowGame->gameTextureAndText.greenSprite.setTexture(windowGame->gameTextureAndText.green);
	windowGame->gameTextureAndText.yellowSprite.setTexture(windowGame->gameTextureAndText.yellow);

	windowGame->gameTextureAndText.blackСheckerSprite.setTexture(windowGame->gameTextureAndText.blackСhecker);
	windowGame->gameTextureAndText.whiteСheckerSprite.setTexture(windowGame->gameTextureAndText.whiteСhecker);
	windowGame->gameTextureAndText.blackСheckerSpriteDM.setTexture(windowGame->gameTextureAndText.blackСheckerDM);
	windowGame->gameTextureAndText.whiteСheckerSpriteDM.setTexture(windowGame->gameTextureAndText.whiteСheckerDM);


	windowGame->gameTextureAndText.newGameButtonSprite.setTexture(windowGame->gameTextureAndText.newGameButton);


	windowGame->gameTextureAndText.txt = new Text;
	windowGame->gameTextureAndText.txt->setFont(windowGame->gameTextureAndText.font);
	windowGame->gameTextureAndText.txt->setCharacterSize(FONT_SIZE);
	windowGame->gameTextureAndText.txt->setFillColor(TEXT_COLOR);
	windowGame->gameTextureAndText.txt->setStyle(TEXT_TYPE);

	windowGame->gameTextureAndText.txtInfo = new Text;
	windowGame->gameTextureAndText.txtInfo->setFont(windowGame->gameTextureAndText.font);
	windowGame->gameTextureAndText.txtInfo->setCharacterSize(FONT_SIZE);
	windowGame->gameTextureAndText.txtInfo->setFillColor(TEXT_COLOR);
	windowGame->gameTextureAndText.txtInfo->setStyle(TEXT_TYPE);
}

void ModelView::StartNewGame()
{
	if (windowGame->gameTextureAndText.newGameButtonSprite.getGlobalBounds().contains(windowGame->e.mouseButton.x, windowGame->e.mouseButton.y))
		game->NewGame();
}

void ModelView::UpdateFromGame()
{
	//Заполнить графическое поле значениями
	for (int i = 0; i < FILD_SIZE; i++)
		for (int j = 0; j < FILD_SIZE; j++)
			windowGame->gameBoard.gridView[i][j] = (((j + i) % 2 == 0) ? CellType::White : CellType::Blue);

	//Заполнить логическое поле значениями
	for (int i = 0; i < FILD_SIZE; i++)
		for (int j = 0; j < FILD_SIZE; j++)
			windowGame->gameBoard.gridLogic[i][j] = nullptr;
	vector<const Checker*> checkers = game->GetCheckers();
	for (int i = 0; i < checkers.size(); i++)
		windowGame->gameBoard.gridLogic[checkers[i]->coord.x - 1][checkers[i]->coord.y - 1] = checkers[i];

	vector<Coord> moves = game->PossibleMoves(game->GetChosenChecker());
	for (int i = 0; i < moves.size(); i++)
		windowGame->gameBoard.gridView[moves[i].x - 1][moves[i].y - 1] = CellType::Green;
	
	if (game->GetChosenChecker() != nullptr)
		windowGame->gameBoard.gridView[game->GetChosenChecker()->coord.x - 1][game->GetChosenChecker()->coord.y - 1] = CellType::Yellow;
}

void ModelView::Start()
{
	GameBoardInit();

	while (windowGame->app->isOpen())
	{
		windowGame->app->clear(Color::Black);
		Events();
		if (!windowGame->app->isOpen())
			break;
		UpdateFromGame();
		GameBoardUpdate();
		windowGame->app->display();
	}
}

void ModelView::Events()
{
	while (windowGame->app->pollEvent(windowGame->e))
	{
		if (windowGame->e.type == Event::Closed)
			windowGame->app->close();

		if (windowGame->e.type == Event::MouseButtonPressed)
			if (windowGame->e.key.code == Mouse::Left) {
				if (!game->GetInfo().isEnd)
				{
					SelectСheckers();
				}
				else
				{
					StartNewGame();
				}
			}
	}
}


void ModelView::GameBoardInit()
{
	UpdateFromGame();
}

void ModelView::GameBoardUpdate()
{
	//Отрисовка статичных спрайтов (прямоугольники)
	GameBoardStaticUpdate();
	
	//Отрисовка букв и цифр
	GameBoardTextUpdate();
	
	//рисование определённых квадратов на поле и шашек
	GameBoardAndСheckersUpdate();
}

void ModelView::GameBoardTextUpdate()
{
	//координаты для букв
	int digitStartPosLeftX = windowGame->gameBoard.cellSize - windowGame->gameBoard.cellSize / 3;
	int digitStartPosRightX = (windowGame->gameBoard.cellSize * 9) + windowGame->gameBoard.cellSize / 6;
	//координаты для букв
	int charPosX = windowGame->gameBoard.cellSize + windowGame->gameBoard.cellSize / 3;
	int	charStartPosTopY = windowGame->gameBoard.cellSize / 2;
	int	charStartPosBottomY = (windowGame->gameBoard.cellSize * 9);

	for (int i = 1, digitPosY = (windowGame->gameBoard.cellSize + (windowGame->gameBoard.cellSize / 5)); i < 9; i += 1, digitPosY += windowGame->gameBoard.cellSize)
	{
		windowGame->gameTextureAndText.txt->setPosition(digitStartPosLeftX, digitPosY);
		windowGame->gameTextureAndText.txt->setString(std::to_string(i));
		windowGame->app->draw(*windowGame->gameTextureAndText.txt);

		windowGame->gameTextureAndText.txt->setPosition(digitStartPosRightX, digitPosY);
		windowGame->gameTextureAndText.txt->setString(std::to_string(i));
		windowGame->app->draw(*windowGame->gameTextureAndText.txt);
	}

	for (char ch = 'a'; ch < 'i'; ch += 1, charPosX += windowGame->gameBoard.cellSize)
	{
		windowGame->gameTextureAndText.txt->setPosition(charPosX, charStartPosTopY);
		windowGame->gameTextureAndText.txt->setString(ch);
		windowGame->app->draw(*windowGame->gameTextureAndText.txt);

		windowGame->gameTextureAndText.txt->setPosition(charPosX, charStartPosBottomY);
		windowGame->gameTextureAndText.txt->setString(ch);
		windowGame->app->draw(*windowGame->gameTextureAndText.txt);
	}

	windowGame->gameTextureAndText.txtInfo->setPosition((windowGame->gameBoard.cellSize * 9) + 52, (windowGame->gameBoard.cellSize / 2) + 250);
	if (game->GetInfo().isEnd)
	{
		windowGame->gameTextureAndText.txtInfo->setString("Конец игры");
		windowGame->gameTextureAndText.txtInfo->setPosition((windowGame->gameBoard.cellSize * 9) + 52, (windowGame->gameBoard.cellSize / 2) + 200);
		windowGame->gameTextureAndText.txtInfo->setString((string)"Победитель:" + ((game->GetInfo().winner == Team::Black) ? "Чёрные" : "Белые"));
		windowGame->app->draw(*windowGame->gameTextureAndText.txtInfo);
	}
	else
	{
		windowGame->gameTextureAndText.txtInfo->setString("Идёт игра");
	}
	windowGame->app->draw(*windowGame->gameTextureAndText.txtInfo);


	windowGame->gameTextureAndText.txtInfo->setPosition((windowGame->gameBoard.cellSize * 9) + 52, (windowGame->gameBoard.cellSize / 2));
	windowGame->gameTextureAndText.txtInfo->setString("Шашек на доске: " + to_string(game->GetInfo().allCheckers));
	windowGame->app->draw(*windowGame->gameTextureAndText.txtInfo);

	windowGame->gameTextureAndText.txtInfo->setPosition((windowGame->gameBoard.cellSize * 9) + 52, (windowGame->gameBoard.cellSize / 2) + 50);
	windowGame->gameTextureAndText.txtInfo->setString("Чёрных шашек на доске:" + to_string(game->GetInfo().blackCheckers));
	windowGame->app->draw(*windowGame->gameTextureAndText.txtInfo);

	windowGame->gameTextureAndText.txtInfo->setPosition((windowGame->gameBoard.cellSize * 9) + 52, (windowGame->gameBoard.cellSize / 2) + 100);
	windowGame->gameTextureAndText.txtInfo->setString("Белых шашек на доске:" + to_string(game->GetInfo().whiteCheckers));
	windowGame->app->draw(*windowGame->gameTextureAndText.txtInfo);

	windowGame->gameTextureAndText.txtInfo->setPosition((windowGame->gameBoard.cellSize * 9) + 52, (windowGame->gameBoard.cellSize / 2) + 150);
	windowGame->gameTextureAndText.txtInfo->setString("Всего перемещений:" + to_string(game->GetInfo().countMoves));
	windowGame->app->draw(*windowGame->gameTextureAndText.txtInfo);
}

void ModelView::GameBoardStaticUpdate()
{
	//Создаем белый прямоугольник
	RectangleShape rectangleBoardWhite(Vector2f((windowGame->gameBoard.cellSize * (windowGame->gameBoard.size + 1)), (windowGame->gameBoard.cellSize * (windowGame->gameBoard.size + 1))));
	//Перемещаем его в нижний ряд справа от многоугольника
	rectangleBoardWhite.move((windowGame->gameBoard.cellSize / 2), (windowGame->gameBoard.cellSize / 2));
	//Устанавливаем ему цвет
	rectangleBoardWhite.setFillColor(WHITE);
	//Отрисовка прямоугольника
	windowGame->app->draw(rectangleBoardWhite);

	//Создаем белый прямоугольник для текста
	RectangleShape rectangleBoardWhiteInfo(Vector2f(330, 630));
	//Перемещаем его в нижний ряд справа от многоугольникаa
	rectangleBoardWhiteInfo.move((windowGame->gameBoard.cellSize * 9) + 50, (windowGame->gameBoard.cellSize / 2));
	//Устанавливаем ему цвет
	rectangleBoardWhiteInfo.setFillColor(WHITE);
	//Отрисовка прямоугольника
	windowGame->app->draw(rectangleBoardWhiteInfo);

	if (game->GetInfo().isEnd)
	{
		windowGame->gameTextureAndText.newGameButtonSprite.setPosition((windowGame->gameBoard.cellSize * 9) + 150, (windowGame->gameBoard.cellSize / 2) + 300);
		windowGame->app->draw(windowGame->gameTextureAndText.newGameButtonSprite);
	}

	//Создаем чёрный прямоугольник (5 и 10 зависимы и отвечают за размер рамки)
	RectangleShape rectangleBoardBlack(Vector2f(windowGame->gameBoard.cellSize * windowGame->gameBoard.size + 10, windowGame->gameBoard.cellSize * windowGame->gameBoard.size + 10));
	//Перемещаем его в нижний ряд справа от многоугольника
	rectangleBoardBlack.move(windowGame->gameBoard.cellSize - 5, windowGame->gameBoard.cellSize - 5);
	//Устанавливаем ему цвет
	rectangleBoardBlack.setFillColor(BLACK);
	//Отрисовка прямоугольника
	windowGame->app->draw(rectangleBoardBlack);
}

void ModelView::GameBoardAndСheckersUpdate()
{
	auto drawSprite = [&](Sprite& sprite, int x, int y)
	{
		// Устанавливаем его в заданную позицию...
		sprite.setPosition(x, y);
		//	Отрисовывание
		windowGame->app->draw(sprite);
	};
	int x, y;

	for (int i = 0; i < FILD_SIZE; i++)
		for (int j = 0; j < FILD_SIZE; j++)
		{
			x = (i + 1) * windowGame->gameBoard.cellSize;
			y = (j + 1) * windowGame->gameBoard.cellSize;
			switch (windowGame->gameBoard.gridView[i][j])
			{
			case CellType::Blue:
				drawSprite(windowGame->gameTextureAndText.blueSprite, x, y);
				break;
			case CellType::White:
				drawSprite(windowGame->gameTextureAndText.whiteSprite, x, y);
				break;
			case CellType::Green:
				drawSprite(windowGame->gameTextureAndText.greenSprite, x, y);
				break;
			case CellType::Yellow:
				drawSprite(windowGame->gameTextureAndText.yellowSprite, x, y);
				break;
			}

			if (windowGame->gameBoard.gridLogic[i][j] == nullptr)
				continue;

			//рисование шашек на поле

			x = (i + 1) * windowGame->gameBoard.cellSize + 7;
			y = (j + 1) * windowGame->gameBoard.cellSize + 7;
			switch (windowGame->gameBoard.gridLogic[i][j]->team)
			{
			case Team::White:
				drawSprite(windowGame->gameTextureAndText.whiteСheckerSprite, x, y);
				break;
			case Team::Black:
				drawSprite(windowGame->gameTextureAndText.blackСheckerSprite, x, y);
				break;
			}
		}
}

void ModelView::SelectСheckers()
{
	int fx = windowGame->e.mouseButton.x / 70, fy = windowGame->e.mouseButton.y / 70;
	Log::Write("(" + to_string(fx) + ";" + to_string(fy) + ")");
	game->Action(Coord(fx, fy));
}