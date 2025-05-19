#include <SFML/Graphics.hpp>
#include "Theme.h"
#include "Tile.h"
#include "Board.h"
#include "GameStates.h"
#include "BotLogic.h"
auto main() -> int {

    auto const arial = sf::Font ("ARIAL.TTF");
    auto gameState = GameState::InMenu;
    auto window = sf::RenderWindow(
        sf::VideoMode({1500, 1000}), "Test",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );


    auto board = Board();

    board.map = createBaseMap();
    board.hexLayout = createBaseHexLayout(board.map);
    auto possibleMoves = std::vector<Position>();
    auto score = std::pair<int,int>{0,0};
    auto turn = Turn::P1Choose;
    auto closerPositions = std::vector<Position>();
    auto furtherPositions = std::vector<Position>();
    auto isGameOn = true;
    Position chosenPosition = {0,0};
    auto savedGames = std::vector<Save>();
    //back to menu btn
    auto quitBtn = sf::RectangleShape({300, 50});
    auto quitText = sf::Text(Theme::board::arial, "back to menu", 26);

    quitBtn.setPosition({
    (window.getSize().x - quitBtn.getSize().x) - 10,
        window.getSize().y - quitBtn.getSize().y - 10
});
    // https://en.sfml-dev.org/forums/index.php?topic=29521.0 centering text inside shapes
    quitText.setOrigin(quitText.getGlobalBounds().size / 2.f + quitText.getLocalBounds().position);
    quitText.setPosition(quitBtn.getPosition() + (quitBtn.getSize() / 2.f));
    quitBtn.setFillColor(Theme::playerTwo::pawn);
    quitText.setFillColor(Theme::playerOne::pawn);
    quitBtn.setOutlineColor(Theme::board::outline);
    quitBtn.setOutlineThickness(3);

    auto saveBtn = sf::RectangleShape({300, 50});
    auto saveText = sf::Text(Theme::board::arial, "save game", 26);

    saveBtn.setPosition({
        (window.getSize().x - saveBtn.getSize().x) - 10,
        quitBtn.getPosition().y - saveBtn.getSize().y - 10
    });
    saveText.setOrigin(saveText.getGlobalBounds().size / 2.f + saveText.getLocalBounds().position);
    saveText.setPosition(saveBtn.getPosition() + (saveBtn.getSize() / 2.f));
    saveBtn.setFillColor(Theme::playerTwo::pawn);
    saveText.setFillColor(Theme::playerOne::pawn);
    saveBtn.setOutlineColor(Theme::board::outline);
    saveBtn.setOutlineThickness(3);





    //setup do menu
    auto menu = std::vector<std::pair<sf::RectangleShape, sf::Text>>();

    auto pvpText = sf::Text(Theme::board::arial, "player vs player", 26);
    auto pvpBtn = sf::RectangleShape({300,50});
    menu.push_back({pvpBtn,pvpText});

    auto pvcText = sf::Text(Theme::board::arial, "player vs computer", 26);
    auto pvcBtn = sf::RectangleShape({300,50});
    menu.push_back({pvcBtn,pvcText});

    auto sgText = sf::Text(Theme::board::arial, "saved games", 26);
    auto sgBtn = sf::RectangleShape({300,50});
    menu.push_back({sgBtn,sgText});

    auto bgText = sf::Text(Theme::board::arial, "best games", 26);
    auto bgBtn = sf::RectangleShape({300,50});
    menu.push_back({bgBtn,bgText});

    auto qText = sf::Text(Theme::board::arial, "quit", 26);
    auto qBtn = sf::RectangleShape({300,50});
    menu.push_back({qBtn,qText});

    auto startingPoint = 150.f;

    for (auto& menuItem: menu) {
        auto& button = menuItem.first;
        auto& text = menuItem.second;

        button.setFillColor(Theme::playerTwo::pawn);
        text.setFillColor(Theme::playerOne::pawn);
        button.setOutlineColor(Theme::board::outline);
        button.setOutlineThickness(3);


        button.setPosition({
            (window.getSize().x - button.getSize().x) / 2.f,
                startingPoint
        });
        // https://en.sfml-dev.org/forums/index.php?topic=29521.0 centering text inside shapes
        text.setOrigin(text.getGlobalBounds().size / 2.f + text.getLocalBounds().position);
        text.setPosition(button.getPosition() + (button.getSize() / 2.f));
        startingPoint += 60;
    }


    while (window.isOpen()) {
        window.clear(Theme::board::background);

        //rysowanie mapy i podswietlanie pionkow gdy pvp
        if (gameState == GameState::InPvp) {
            score = getScore(board.map);
            if (score.first == 0 || score.second == 0 || score.first + score.second == 58)
                isGameOn = false;

            //wynik
            auto p1Score = sf::Text(arial, std::to_string(score.first), 50);
            p1Score.setPosition({10, 10});
            p1Score.setFillColor(Theme::playerOne::score);
            auto p2Score = sf::Text(arial, std::to_string(score.second), 50);
            p2Score.setPosition({100, 10});
            p2Score.setFillColor(Theme::playerTwo::score);
            window.draw(p1Score);
            window.draw(p2Score);

            //podswietlanie opcji pionkow
            if (isGameOn) {
                if (turn == Turn::P1Choose) {
                    resetOutlines(board.hexLayout);
                    auto playerPawns = getPlayerOneOptions(board.map);
                    for (const auto &playerPawn: playerPawns) {
                        if (!isLockedPawn(playerPawn.position, board.map))
                            board.hexLayout[playerPawn.position.column][playerPawn.position.row].setOutlineColor(
                                Theme::board::option);
                    }
                } else if (turn == Turn::P2Choose) {
                    resetOutlines(board.hexLayout);
                    auto playerPawns = getPlayerTwoOptions(board.map);
                    for (const auto &playerPawn: playerPawns) {
                        if (!isLockedPawn(playerPawn.position, board.map))
                            board.hexLayout[playerPawn.position.column][playerPawn.position.row].setOutlineColor(
                                Theme::board::option);
                    }
                }
            } else resetOutlines(board.hexLayout);

            //rysowanie mapy od nowa.
            for (auto & col: board.map) {
                for (auto & tile : col) {
                    window.draw(board.hexLayout[tile.position.column][tile.position.row]);
                }
            }

            //rysowanie back to menu btn
            window.draw(quitBtn);
            window.draw(quitText);
            window.draw(saveBtn);
            window.draw(saveText);
        }
        if (gameState == GameState::InMenu) {
            for (auto &menuItem: menu) {
                window.draw(menuItem.first);
                window.draw(menuItem.second);
            }
        }
        if (gameState == GameState::InPvc) {
            score = getScore(board.map);
            if (score.first == 0 || score.second == 0 || score.first + score.second == 58)
                isGameOn = false;

            //wynik
            auto p1Score = sf::Text(arial, std::to_string(score.first), 50);
            p1Score.setPosition({10, 10});
            p1Score.setFillColor(Theme::playerOne::score);
            auto p2Score = sf::Text(arial, std::to_string(score.second), 50);
            p2Score.setPosition({100, 10});
            p2Score.setFillColor(Theme::playerTwo::score);
            window.draw(p1Score);
            window.draw(p2Score);



            //podswietlanie opcji pionkow
            if (isGameOn) {
                if (turn == Turn::P1Choose) {
                    resetOutlines(board.hexLayout);
                    auto playerPawns = getPlayerOneOptions(board.map);
                    for (const auto &playerPawn: playerPawns) {
                        if (!isLockedPawn(playerPawn.position, board.map))
                            board.hexLayout[playerPawn.position.column][playerPawn.position.row].setOutlineColor(
                                Theme::board::option);
                    }
                }
                if (turn == Turn::P2Choose) {
                    auto move = getBestMove(board, State::Player2);
                    board.map = makeMove(board.map,
                                         board.map[move.first.position.column][move.first.position.row],
                                         board.map[move.second.position.column][move.second.position.row]);

                    board.hexLayout = createBaseHexLayout(board.map);

                    turn = Turn::P1Choose;
                }
            }

            //rysowanie mapy od nowa.
            for (int i = 0; i < board.hexLayout.size(); ++i) {
                for (int j = 0; j < board.hexLayout[i].size(); ++j) {
                    window.draw(board.hexLayout[i][j]);
                }
            }

            //rysowanie back to menu btn
            window.draw(quitBtn);
            window.draw(quitText);
            window.draw(saveBtn);
            window.draw(saveText);


        }
        if (gameState == GameState::InSavedGames) {
            for (auto const save : savedGames) {
                window.draw(save.button);
                window.draw(save.text);
            }
            window.draw(quitBtn);
            window.draw(quitText);
        }


        while (auto const event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() || gameState == GameState::Quit) {
                window.close();
            }
            //obsluga zdarzen gdy pvp
            if (gameState == GameState::InPvp) {
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                        if (quitBtn.getGlobalBounds().contains(
                            static_cast<sf::Vector2<float>>(event->getIf<sf::Event::MouseButtonPressed>()->position))) {
                            gameState = GameState::InMenu;
                        }
                        if (saveBtn.getGlobalBounds().contains(
                            static_cast<sf::Vector2<float>>(event->getIf<sf::Event::MouseButtonPressed>()->position))) {
                            auto name = findNewFileName();
                            saveBoardToTextFile(board, turn, name);
                        }
                    }
                }

                if (isGameOn) {
                    if (turn == Turn::P1Choose) {
                        //p1 wybiera pole z którego wykona swój ruch
                        if (event->is<sf::Event::MouseButtonPressed>()) {
                            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            auto position = static_cast<sf::Vector2f>(mouseEvent->position);

                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                for (auto pawn: getPlayerOneOptions(board.map)) {
                                    if (board.hexLayout[pawn.position.column][pawn.position.row].getGlobalBounds().
                                        contains(position) && !isLockedPawn(pawn.position, board.map)) {
                                        board.hexLayout[pawn.position.column][pawn.position.row].setOutlineColor(
                                            Theme::board::chosen);
                                        closerPositions = findCloserPositions({
                                            pawn.position.column, pawn.position.row
                                        });
                                        furtherPositions = findFurtherPositions({
                                            pawn.position.column, pawn.position.row
                                        });
                                        chosenPosition = pawn.position;
                                        resetOutlines(board.hexLayout);
                                        for (auto closer_position: closerPositions) {
                                            if (board.map[closer_position.column][closer_position.row].state ==
                                                State::Empty)
                                                board.hexLayout[closer_position.column][closer_position.row].
                                                        setOutlineColor(Theme::playerOne::closer);
                                        }
                                        for (auto further_position: furtherPositions) {
                                            if (board.map[further_position.column][further_position.row].state ==
                                                State::Empty)
                                                board.hexLayout[further_position.column][further_position.row].
                                                        setOutlineColor(Theme::playerOne::further);
                                        }
                                        turn = Turn::P1Move;
                                    }
                                }
                            }
                        }
                    }
                    if (turn == Turn::P1Move) {
                        if (event->is<sf::Event::MouseButtonPressed>()) {
                            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            auto position = static_cast<sf::Vector2f>(mouseEvent->position);

                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                for (auto pawn: closerPositions) {
                                    if (board.hexLayout[pawn.column][pawn.row].getGlobalBounds().contains(position) &&
                                        board.map[pawn.column][pawn.row].state == State::Empty) {
                                        board.map[pawn.column][pawn.row].state = State::Player1;
                                        // przejecie enemy pionkow
                                        for (auto neighbour: findCloserPositions(pawn)) {
                                            if (board.map[neighbour.column][neighbour.row].state == State::Player2) {
                                                updateState(board.hexLayout[neighbour.column][neighbour.row],
                                                            board.map[neighbour.column][neighbour.row], State::Player1);
                                            }
                                        }
                                        board.hexLayout[pawn.column][pawn.row].setFillColor(Theme::playerOne::pawn);
                                        turn = Turn::P2Choose;
                                        break;
                                    }
                                }
                                for (auto pawn: furtherPositions) {
                                    if (board.hexLayout[pawn.column][pawn.row].getGlobalBounds().contains(position) &&
                                        board.map[pawn.column][pawn.row].state == State::Empty) {
                                        board.map[pawn.column][pawn.row].state = State::Player1;
                                        // przejecie enemy pionkow
                                        for (auto neighbour: findCloserPositions(pawn)) {
                                            if (board.map[neighbour.column][neighbour.row].state == State::Player2) {
                                                updateState(board.hexLayout[neighbour.column][neighbour.row],
                                                            board.map[neighbour.column][neighbour.row], State::Player1);
                                            }
                                        }
                                        board.hexLayout[pawn.column][pawn.row].setFillColor(Theme::playerOne::pawn);
                                        board.map[chosenPosition.column][chosenPosition.row].state = State::Empty;
                                        board.hexLayout[chosenPosition.column][chosenPosition.row].setFillColor(
                                            Theme::board::empty);
                                        turn = Turn::P2Choose;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if (turn == Turn::P2Choose) {
                        //p2 wybiera pole z którego wykona swój ruch
                        if (event->is<sf::Event::MouseButtonPressed>()) {
                            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            auto position = static_cast<sf::Vector2f>(mouseEvent->position);

                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                for (auto pawn: getPlayerTwoOptions(board.map)) {
                                    if (board.hexLayout[pawn.position.column][pawn.position.row].getGlobalBounds().
                                        contains(position) && !isLockedPawn(pawn.position, board.map)) {
                                        board.hexLayout[pawn.position.column][pawn.position.row].setOutlineColor(
                                            Theme::board::option);
                                        closerPositions = findCloserPositions({
                                            pawn.position.column, pawn.position.row
                                        });
                                        furtherPositions = findFurtherPositions({
                                            pawn.position.column, pawn.position.row
                                        });
                                        chosenPosition = pawn.position;
                                        resetOutlines(board.hexLayout);
                                        for (auto closer_position: closerPositions) {
                                            if (board.map[closer_position.column][closer_position.row].state ==
                                                State::Empty)
                                                board.hexLayout[closer_position.column][closer_position.row].
                                                        setOutlineColor(Theme::playerTwo::closer);
                                        }
                                        for (auto further_position: furtherPositions) {
                                            if (board.map[further_position.column][further_position.row].state ==
                                                State::Empty)
                                                board.hexLayout[further_position.column][further_position.row].
                                                        setOutlineColor(Theme::playerTwo::further);
                                        }
                                        turn = Turn::P2Move;
                                    }
                                }
                            }
                        }
                    }
                    if (turn == Turn::P2Move) {
                        if (event->is<sf::Event::MouseButtonPressed>()) {
                            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            auto position = static_cast<sf::Vector2f>(mouseEvent->position);
                            Position chosenTilePosition = {0, 0};

                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                for (auto pawn: closerPositions) {
                                    if (board.hexLayout[pawn.column][pawn.row].getGlobalBounds().contains(position) &&
                                        board.map[pawn.column][pawn.row].state == State::Empty) {
                                        board.map[pawn.column][pawn.row].state = State::Player2;
                                        // przejecie enemy pionkow
                                        for (auto neighbour: findCloserPositions(pawn)) {
                                            if (board.map[neighbour.column][neighbour.row].state == State::Player1) {
                                                updateState(board.hexLayout[neighbour.column][neighbour.row],
                                                            board.map[neighbour.column][neighbour.row], State::Player2);
                                            }
                                        }
                                        board.hexLayout[pawn.column][pawn.row].setFillColor(Theme::playerTwo::pawn);
                                        chosenTilePosition = pawn;
                                        turn = Turn::P1Choose;
                                        break;
                                    }
                                }
                                for (auto pawn: furtherPositions) {
                                    if (board.hexLayout[pawn.column][pawn.row].getGlobalBounds().contains(position) &&
                                        board.map[pawn.column][pawn.row].state == State::Empty) {
                                        board.map[pawn.column][pawn.row].state = State::Player2;
                                        // przejecie enemy pionkow
                                        for (auto neighbour: findCloserPositions(pawn)) {
                                            if (board.map[neighbour.column][neighbour.row].state == State::Player1) {
                                                updateState(board.hexLayout[neighbour.column][neighbour.row],
                                                            board.map[neighbour.column][neighbour.row], State::Player2);
                                            }
                                        }
                                        board.hexLayout[pawn.column][pawn.row].setFillColor(Theme::playerTwo::pawn);
                                        board.map[chosenPosition.column][chosenPosition.row].state = State::Empty;
                                        board.hexLayout[chosenPosition.column][chosenPosition.row].setFillColor(
                                            Theme::board::empty);
                                        chosenTilePosition = pawn;
                                        turn = Turn::P1Choose;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //obsluga zdarzen gdy w menu
            if (gameState == GameState::InMenu) {
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                        int i = 0;
                        for (auto &menuItem: menu) {
                            auto &button = menuItem.first;
                            auto position = event->getIf<sf::Event::MouseButtonPressed>()->position;

                            if (button.getGlobalBounds().contains(static_cast<sf::Vector2<float>>(position))) {
                                switch (i) {
                                    case 0: {
                                        gameState = GameState::InPvp;
                                        board.map = createBaseMap();
                                        board.hexLayout = createBaseHexLayout(board.map);
                                        possibleMoves = std::vector<Position>();
                                        score = std::pair<int,int>{0,0};
                                        turn = Turn::P1Choose;
                                        closerPositions = std::vector<Position>();
                                        furtherPositions = std::vector<Position>();
                                        isGameOn = true;
                                        chosenPosition = {0,0};
                                        break;
                                    }
                                    case 1: {
                                        gameState = GameState::InPvc;
                                        board.map = createBaseMap();
                                        board.hexLayout = createBaseHexLayout(board.map);
                                        possibleMoves = std::vector<Position>();
                                        score = std::pair<int,int>{0,0};
                                        turn = Turn::P1Choose;
                                        closerPositions = std::vector<Position>();
                                        furtherPositions = std::vector<Position>();
                                        isGameOn = true;
                                        chosenPosition = {0,0};
                                        break;
                                    }
                                    case 2: {
                                        savedGames.clear();
                                        for (auto const entry : std::filesystem::directory_iterator("/Users/dawidleczycki/CLionProjects/HEX-GAME/cmake-build-debug/savedGames")) {

                                            auto text = sf::Text(arial, entry.path().filename().string(), 26);
                                            auto button = sf::RectangleShape({300,50});
                                            auto fileName = entry.path().filename().string();

                                            savedGames.push_back(Save(text, button, fileName));
                                        }

                                        auto startingPoint = 50.f;

                                        for (auto& saveItem: savedGames) {
                                            auto& button = saveItem.button;
                                            auto& text = saveItem.text;

                                            button.setFillColor(Theme::playerTwo::pawn);
                                            text.setFillColor(Theme::playerOne::pawn);
                                            button.setOutlineColor(Theme::board::outline);
                                            button.setOutlineThickness(3);


                                            button.setPosition({
                                                (window.getSize().x - button.getSize().x) / 2.f,
                                                    startingPoint
                                            });
                                            // https://en.sfml-dev.org/forums/index.php?topic=29521.0 centering text inside shapes
                                            text.setOrigin(text.getGlobalBounds().size / 2.f + text.getLocalBounds().position);
                                            text.setPosition(button.getPosition() + (button.getSize() / 2.f));
                                            startingPoint += 60;
                                        }



                                        gameState = GameState::InSavedGames;
                                        break;
                                    }
                                    case 3: gameState = GameState::InBestGames; break;
                                    case 4: gameState = GameState::Quit;
                                        break;
                                }
                            }
                            i++;
                        }
                    }
                }
            }

            //obsluga zdarzen gdy w pvc
            if (gameState == GameState::InPvc) {
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                        if (quitBtn.getGlobalBounds().contains(
                            static_cast<sf::Vector2<float>>(event->getIf<sf::Event::MouseButtonPressed>()->position))) {
                            gameState = GameState::InMenu;
                        }
                        if (saveBtn.getGlobalBounds().contains(
                            static_cast<sf::Vector2<float>>(event->getIf<sf::Event::MouseButtonPressed>()->position))) {
                            auto name = findNewFileName();
                            saveBoardToTextFile(board, turn, name);
                            }
                    }
                }

                if (isGameOn) {
                    if (turn == Turn::P1Choose) {
                        //p1 wybiera pole z którego wykona swój ruch
                        if (event->is<sf::Event::MouseButtonPressed>()) {
                            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            auto position = static_cast<sf::Vector2f>(mouseEvent->position);

                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                for (auto pawn: getPlayerOneOptions(board.map)) {
                                    if (board.hexLayout[pawn.position.column][pawn.position.row].getGlobalBounds().
                                        contains(position) && !isLockedPawn(pawn.position, board.map)) {
                                        board.hexLayout[pawn.position.column][pawn.position.row].setOutlineColor(
                                            Theme::board::chosen);
                                        closerPositions = findCloserPositions({
                                            pawn.position.column, pawn.position.row
                                        });
                                        furtherPositions = findFurtherPositions({
                                            pawn.position.column, pawn.position.row
                                        });
                                        chosenPosition = pawn.position;
                                        resetOutlines(board.hexLayout);
                                        for (auto closer_position: closerPositions) {
                                            if (board.map[closer_position.column][closer_position.row].state ==
                                                State::Empty)
                                                board.hexLayout[closer_position.column][closer_position.row].
                                                        setOutlineColor(Theme::playerOne::closer);
                                        }
                                        for (auto further_position: furtherPositions) {
                                            if (board.map[further_position.column][further_position.row].state ==
                                                State::Empty)
                                                board.hexLayout[further_position.column][further_position.row].
                                                        setOutlineColor(Theme::playerOne::further);
                                        }
                                        turn = Turn::P1Move;
                                    }
                                }
                            }
                        }
                    }
                    if (turn == Turn::P1Move) {
                        if (event->is<sf::Event::MouseButtonPressed>()) {
                            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            auto position = static_cast<sf::Vector2f>(mouseEvent->position);

                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                for (auto pawn: closerPositions) {
                                    if (board.hexLayout[pawn.column][pawn.row].getGlobalBounds().contains(position) &&
                                        board.map[pawn.column][pawn.row].state == State::Empty) {
                                        board.map[pawn.column][pawn.row].state = State::Player1;
                                        // przejecie enemy pionkow
                                        for (auto neighbour: findCloserPositions(pawn)) {
                                            if (board.map[neighbour.column][neighbour.row].state == State::Player2) {
                                                updateState(board.hexLayout[neighbour.column][neighbour.row],
                                                            board.map[neighbour.column][neighbour.row], State::Player1);
                                            }
                                        }
                                        board.hexLayout[pawn.column][pawn.row].setFillColor(Theme::playerOne::pawn);
                                        turn = Turn::P2Choose;
                                        break;
                                    }
                                }
                                for (auto pawn: furtherPositions) {
                                    if (board.hexLayout[pawn.column][pawn.row].getGlobalBounds().contains(position) &&
                                        board.map[pawn.column][pawn.row].state == State::Empty) {
                                        board.map[pawn.column][pawn.row].state = State::Player1;
                                        // przejecie enemy pionkow
                                        for (auto neighbour: findCloserPositions(pawn)) {
                                            if (board.map[neighbour.column][neighbour.row].state == State::Player2) {
                                                updateState(board.hexLayout[neighbour.column][neighbour.row],
                                                            board.map[neighbour.column][neighbour.row], State::Player1);
                                            }
                                        }
                                        board.hexLayout[pawn.column][pawn.row].setFillColor(Theme::playerOne::pawn);
                                        board.map[chosenPosition.column][chosenPosition.row].state = State::Empty;
                                        board.hexLayout[chosenPosition.column][chosenPosition.row].setFillColor(
                                            Theme::board::empty);
                                        turn = Turn::P2Choose;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (gameState == GameState::InSavedGames) {
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                        auto position = event->getIf<sf::Event::MouseButtonPressed>()->position;
                        for (auto & saveItem : savedGames) {
                            if (saveItem.button.getGlobalBounds().contains(static_cast<sf::Vector2<float>>(position))) {
                                gameState = GameState::InPvp;
                                board = loadBoardFromFile("/Users/dawidleczycki/CLionProjects/HEX-GAME/cmake-build-debug/savedGames/" + saveItem.fileName).first;
                                turn = loadBoardFromFile("/Users/dawidleczycki/CLionProjects/HEX-GAME/cmake-build-debug/savedGames/" + saveItem.fileName).second;
                                break;
                            }
                        }
                        if (quitBtn.getGlobalBounds().contains(static_cast<sf::Vector2<float>>(position))) {
                            gameState = GameState::InMenu;
                        }
                    }
                }
            }
        }
        window.display();
    }
}

