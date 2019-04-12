#include "Game.h"

static const int GRAPH_SIZE = 80;

/// <summary>
/// Constructor for the game object
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1920, 1080, 32 }, "Neuroevolution Demonstration" },
	m_screenRatioX{ 1.f },
	m_screenRatioY{ 1.f },
	m_bird{ 250, 400 },
	m_trainAi{ true },
	m_memberNo{ 0 },
	m_score{ 0 },
	m_generations{ 0 },
	m_inputPopulation{ 200 },
	m_inputHiddenLayerWidth{ 5 },
	m_inputElitism{ 2 },
	m_inputCrossoverRate{ 0.9f },
	m_inputMutationRate{ 0.1f },
	m_inputFrameRateDivisor{ 60 },
	m_inputGenerationsToSkip{ 0 },
	m_inputSkipGenerations{ false },
	m_inputScoreThreshold{ 10000 },
	m_inputVisionMode{ true },
	m_inputShowVisionColliders{ true },
	m_inputDrawGame{ true },
	m_actualVisionMode{ m_inputVisionMode },
	m_currentAverage{ 0 },
	m_currentMax{ 0 },
	m_highestFitness{ 0 },
	m_highestAverage{ 0 },
	m_runningSum{ 0 },
	m_outputPos{ 1280, 0 },
	m_outputSize{ 640, 1080 },
	m_outputScale{ 2.f },
	m_parameterPos{ 0, 720 },
	m_parameterSize{ 1280, 360 },
	m_parameterScale{ 1.7f }
{
	if (!m_gameRenderTexture.create(1280, 720))
	{
		std::cout << "Error: Could not create render texture" << std::endl;
		return;
	}
	ImGui::SFML::Init(m_window);
	reset();
	if (m_trainAi)
	{
		setupANN();
	}
	float visionRectHeight = GAME_SCREEN_HEIGHT / 9.f;
	for (int i = 0; i < 9; ++i)
	{
		VisionRect * vr = new VisionRect();
		vr->colliding = false;
		sf::RectangleShape shape;
		shape.setPosition(250, i * visionRectHeight);
		shape.setSize(sf::Vector2f(175, visionRectHeight));
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(4.f);
		vr->rect = shape;
		m_visionRects.push_back(vr);
	}
}

/// <summary>
/// Destructor for the game object
/// </summary>
Game::~Game() 
{
	if (m_trainAi)
	{
		delete m_ann;
		delete m_pool;
	}
}

/// <summary>
/// Run the Game
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Clock uiClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	timePerFrame = sf::seconds(1.f / m_inputFrameRateDivisor); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // As many as possible
		timeSinceLastUpdate += clock.restart();
		if (m_inputSkipGenerations)
		{
			timeSinceLastUpdate = sf::seconds(0);
			update(timePerFrame);
		}
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // At least 60 fps
			update(timePerFrame); // 60 fps
		}
		if (uiClock.getElapsedTime() > sf::seconds(1.f / 60.f))
		{
			ImGui::SFML::Update(m_window, uiClock.restart());
			timePerFrame = sf::seconds(1.f / m_inputFrameRateDivisor);
			renderImgui();
		}
	}
}

/// <summary>
/// Process the game events
/// </summary>
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (sf::Event::Closed == event.type) // window Closed
		{
			m_window.close();
		}
		if (sf::Event::Resized == event.type) // Window Resized
		{
			m_screenRatioX = m_window.getSize().x / static_cast<float>(SCREEN_WIDTH);
			m_screenRatioY = m_window.getSize().y / static_cast<float>(SCREEN_HEIGHT);
		}
		if (sf::Event::KeyPressed == event.type) // User Key Press
		{
			if (sf::Keyboard::Escape == event.key.code)
			{
				m_window.close();
			}
		}
	}
}

/// <summary>
/// Update loop for the game
/// </summary>
/// <param name="t_deltaTime">deltatime</param>
void Game::update(sf::Time t_deltaTime)
{
	// Game Update
	for (int i = 0; i < m_pipePairs.size(); ++i)
	{
		m_pipePairs.at(i).update();
	}
	collisionCheck();

	// Bird Update
	if (m_trainAi)
	{
		feedForwardANN();
	}
	else
	{
		m_bird.update();
	}
	if (!m_bird.isAlive())
	{
		if (m_trainAi)
		{
			nextAi();
		}
		reset();
		m_bird.setAliveStatus(true);
	}
}

void Game::feedForwardANN()
{
	PipePair pair = getClosestPipePair();
	auto nodes = m_ann->getAllNodes();
	m_ann->reset();
	if (nodes.size() > 9)
	{
		nodes["input1"]->setValue(m_bird.getY() / GAME_SCREEN_HEIGHT);
		nodes["input2"]->setValue(m_visionRects.at(0)->colliding);
		nodes["input3"]->setValue(m_visionRects.at(1)->colliding);
		nodes["input4"]->setValue(m_visionRects.at(2)->colliding);
		nodes["input5"]->setValue(m_visionRects.at(3)->colliding);
		nodes["input6"]->setValue(m_visionRects.at(4)->colliding);
		nodes["input7"]->setValue(m_visionRects.at(5)->colliding);
		nodes["input8"]->setValue(m_visionRects.at(6)->colliding);
		nodes["input9"]->setValue(m_visionRects.at(7)->colliding);
		nodes["input10"]->setValue(m_visionRects.at(8)->colliding);
	}
	else
	{
		nodes["input1"]->setValue((pair.getGapY() + 100) / GAME_SCREEN_HEIGHT);
		nodes["input2"]->setValue((pair.getX() - 250.f) / 250.f);
		nodes["input3"]->setValue(m_bird.getY() / GAME_SCREEN_HEIGHT);
	}
	m_bird.updateNN(nodes["output"]->getOutput() > 0.5f);
	m_score++;
	if (m_score > m_inputScoreThreshold)
	{
		m_inputGenerationsToSkip = 0;
		m_inputSkipGenerations = false;
		m_ann->genChromosome().saveToTextFile("Best_Chromosome.txt");
		m_ann->applyChromosome(m_ann->genChromosome());
	}
}

void Game::nextAi()
{
	auto & members = m_pool->getMembers();

	members.at(m_memberNo).fitness = m_score;
	m_runningSum += m_score;
	if (m_score > m_currentMax)
	{
		m_currentMax = m_score;
	}
	m_score = 0;
	++m_memberNo;
	if (m_memberNo < members.size())
	{
		//srand(time(NULL));
		m_ann->reset();
		m_ann->applyChromosome(members.at(m_memberNo).chromosome);
		//srand(0);
	}
	else
	{
		nextGeneration();
	}
}

void Game::nextGeneration()
{
	handleData();
	auto & members = m_pool->getMembers();
	m_memberNo = 0;

	++m_generations;
	std::cout << m_generations << std::endl;
	//srand(time(NULL));
	m_pool->runGeneration(m_inputElitism);
	if (m_inputSkipGenerations && m_inputGenerationsToSkip > 0)
	{
		if (--m_inputGenerationsToSkip <= 0)
		{
			m_inputSkipGenerations = false;
		}
	}
	//srand(0);
	m_ann->reset();
	m_ann->applyChromosome(members.at(m_memberNo).chromosome);
}

void Game::handleData()
{
	if (m_currentMax > m_highestFitness)
	{
		m_highestFitness = m_currentMax;
	}
	m_recentMax.push_back(m_currentMax);
	if (m_recentMax.size() > GRAPH_SIZE)
	{
		m_recentMax.pop_front();
	}
	m_currentAverage = m_runningSum / m_pool->getMembers().size();
	if (m_currentAverage > m_highestAverage)
	{
		m_highestAverage = m_currentAverage;
	}
	m_recenteAvg.push_back(m_currentAverage);
	if (m_recenteAvg.size() > GRAPH_SIZE)
	{
		m_recenteAvg.pop_front();
	}
	m_currentMax = 0;
	m_runningSum = 0;
}

/// <summary>
/// Draw loop for the game
/// </summary>
void Game::render()
{
	if (m_inputDrawGame)
	{
		m_gameRenderTexture.clear(sf::Color::Black);
		if (m_actualVisionMode && m_inputShowVisionColliders)
		{
			for (auto & vr : m_visionRects)
			{
				vr->rect.setFillColor(vr->colliding ? sf::Color::Red : sf::Color::White);
				m_gameRenderTexture.draw(vr->rect);
			}
		}
		for (int i = 0; i < m_pipePairs.size(); ++i)
		{
			m_pipePairs.at(i).renderToTexture(m_gameRenderTexture);
		}
		m_bird.renderToTexture(m_gameRenderTexture);
		m_gameRenderTexture.display();
		sf::Sprite renderSprite;
		renderSprite.setTexture(m_gameRenderTexture.getTexture());
		m_window.draw(renderSprite);
	}
}

void Game::renderImgui()
{
	m_window.clear(sf::Color::Black);
	render();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	renderInputWindow(window_flags);
	renderOutputWindow(window_flags);
	ImGui::SFML::Render(m_window);
	m_window.display();
}

void Game::renderInputWindow(ImGuiWindowFlags window_flags)
{
	ImGui::SetNextWindowPos(sf::Vector2f(m_parameterPos.x * m_screenRatioX, m_parameterPos.y * m_screenRatioY));
	ImGui::SetNextWindowSize(sf::Vector2f(m_parameterSize.x * m_screenRatioX, m_parameterSize.y * m_screenRatioY));
	ImGui::Begin("Population Parameters", NULL, window_flags); // begin window
	ImGui::SetWindowFontScale(m_parameterScale * std::min(m_screenRatioX, m_screenRatioY)); // Increase Window Font Size (necessary for high DPI screens)
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8f);
	ImGui::Text("Neuroevolution");
	renderColor(OPT_POPULATION, m_inputPopulation);
	ImGui::SliderInt("Population", &m_inputPopulation, 20, 1000);
	renderTooltip("The total size of the population");
	renderColor(OPT_ELITISM, m_inputElitism);
	ImGui::SliderInt("Elitism Value", &m_inputElitism, 0, 20);
	renderTooltip("The number of top individuals to accept before including chance");
	renderColor(OPT_CROSSOVER, m_inputCrossoverRate);
	ImGui::SliderFloat("Crossover Rate", &m_inputCrossoverRate, 0.0f, 1.0f, "Crossover Rate = %.3f");
	renderTooltip("The crossover rate of the population");
	renderColor(OPT_MUTATION, m_inputMutationRate);
	ImGui::SliderFloat("Mutation Rate", &m_inputMutationRate, 0.0f, 1.0f, "Mutation Rate = %.3f");
	renderTooltip("The mutation rate of the population");
	ImGui::Text("Other");
	renderColor(60, m_inputFrameRateDivisor);
	ImGui::SliderInt("Frame Rate", &m_inputFrameRateDivisor, 30, 480);
	renderTooltip("Speed at which to run the game");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
	ImGui::InputInt("Distance Threshold", &m_inputScoreThreshold);
	renderTooltip("Desired distance to reach before stopping genetic algorithm");
	ImGui::SameLine();
	ImGui::Checkbox("Vision Mode", &m_inputVisionMode);
	renderTooltip("Allow ANN to 'see' pipes");
	ImGui::SameLine();
	ImGui::Checkbox("Draw Game", &m_inputDrawGame);
	ImGui::InputInt("Generations To Run", &m_inputGenerationsToSkip);
	renderTooltip("Number of generations to run at the highest possible speed");
	ImGui::SameLine();
	ImGui::Checkbox("Show Vision Colliders", &m_inputShowVisionColliders);
	renderTooltip("Show pipe colliders");
	if (ImGui::Button("Run Rapid Generations"))
	{
		if (m_inputGenerationsToSkip > 0)
		{
			m_inputSkipGenerations = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		delete m_pool;
		delete m_ann;
		m_actualVisionMode = m_inputVisionMode;
		m_memberNo = 0;
		m_generations = 0;
		reset();
		setupANN();
		for(auto & i : m_recentMax)
		{
			i = 0.f;
		}
		m_highestFitness = 0.f;
		for (auto & i : m_recenteAvg)
		{
			i = 0.f;
		}
		m_highestAverage = 0.f;
		m_inputSkipGenerations = false;
		m_inputGenerationsToSkip = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Run Best Individual"))
	{
		Chromosome c;
		c.loadFromTextFile("Best_Chromosome.txt");
		m_ann->reset();
		delete m_ann;
		m_ann = new ANN(0.5f, 1.f, false);
		m_ann->genFromChromosome(c);
		m_ann->applyChromosome(c);
		reset();
	}
	ImGui::SameLine();
	if (ImGui::Button("Run Saved Individual"))
	{
		Chromosome c;
		c.loadFromTextFile("Saved_Individual.txt");
		m_ann->reset();
		delete m_ann;
		m_ann = new ANN(0.5f, 1.f, false);
		m_ann->genFromChromosome(c);
		m_ann->applyChromosome(c);
		reset();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Individual"))
	{
		Chromosome c = m_ann->genChromosome();
		c.saveToTextFile("Saved_Individual.txt");
	}
	ImGui::End();
}

void Game::renderOutputWindow(ImGuiWindowFlags window_flags)
{
	ImGui::SetNextWindowPos(sf::Vector2f(m_outputPos.x * m_screenRatioX, m_outputPos.y * m_screenRatioY));
	ImGui::SetNextWindowSize(sf::Vector2f(m_outputSize.x * m_screenRatioX, m_outputSize.y * m_screenRatioY));
	ImGui::Begin("Neuroevolution Output", NULL, window_flags);
	ImGui::SetWindowFontScale(m_outputScale * std::min(m_screenRatioX, m_screenRatioY)); // Increase Window Font Size (necessary for high DPI screens)

	float maxes[120] = { 0 };
	int counter = 0;
	for (auto & i : m_recentMax)
	{
		maxes[counter] = i;
		++counter;
	}
	counter = 0;
	float avgs[120] = { 0 };
	for (auto & i : m_recenteAvg)
	{
		avgs[counter] = i;
		++counter;
	}
	ImGui::Text("Average Fitness");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.98f);
	ImGui::PlotLines("", avgs, IM_ARRAYSIZE(avgs), 0, "", 0.f, m_highestAverage, ImVec2(0, 120));
	ImGui::Text("Max Fitness");
	ImGui::PlotLines("", maxes, IM_ARRAYSIZE(maxes), 0, "", 0.f, m_highestFitness, ImVec2(0, 120));
	std::string scoreString = "Current Fitness/Score: " + std::to_string(m_score);
	ImGui::Text(scoreString.c_str());
	std::string highestString = "Highest Fitness Achieved: " + std::to_string(m_highestFitness);
	ImGui::Text(highestString.c_str());
	std::string generationsString = "Current Generation: " + std::to_string(m_generations);
	ImGui::Text(generationsString.c_str());
	ImGui::End();
}

void Game::renderColor(float optimal, float val)
{
	float multiple = val / optimal;
	multiple = multiple > 1.f ? optimal / val : multiple;
	float green = 255 * multiple;
	float red = 255 * (1.f - multiple);
	ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
	ImGui::ColorButton("", sf::Color(red, green, 0), flags);
	ImGui::SameLine();
}

void Game::renderTooltip(std::string text)
{
	ImGui::SameLine(); 
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::SetWindowFontScale(m_outputScale * std::min(m_screenRatioX, m_screenRatioY)); // Increase Window Font Size (necessary for high DPI screens)
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(text.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Game::collisionCheck()
{
	sf::FloatRect birdRect = m_bird.getShape().getGlobalBounds();
	if (birdRect.top > GAME_SCREEN_HEIGHT || birdRect.top < -30)
	{
		m_bird.setAliveStatus(false);
		return;
	}
	for (auto & vr : m_visionRects)
	{
		vr->colliding = false;
	}
	for (PipePair & pair : m_pipePairs)
	{
 		if (m_bird.isAlive() && pair.checkCollision(birdRect))
		{
 			m_bird.setAliveStatus(false);
		}
		for (auto & vr : m_visionRects)
		{
			if (!vr->colliding)
			{
				vr->colliding = pair.checkCollision(vr->rect.getGlobalBounds());
			}
		}
	}
}

void Game::reset()
{
	m_score = 0;
	m_bird.setPosition(sf::Vector2f(250, 400));
	m_pipePairs.clear();
	float startX = 400;
	float gap = 250;
	srand(0);
	for (int i = 0; i < 8; ++i)
	{
		PipePair newPair(startX + gap * i, GAME_SCREEN_HEIGHT, 8, gap);
		m_pipePairs.push_back(newPair);
	}
	srand(time(NULL));
}

void Game::setupANN()
{
	m_ann = new ANN(0.5f, 1.f, false);
	Chromosome seed;
	if (m_actualVisionMode)
	{
		seed.loadFromTextFile("Vision_Mode.txt");
	}
	else
	{
		seed.loadFromTextFile("Normal_Mode.txt");
	}
	m_ann->genFromChromosome(seed);
	m_pool = new Pool(m_ann->genChromosome(), m_inputPopulation, m_inputCrossoverRate, m_inputMutationRate, -1.5f, 1.5f);
	m_ann->applyChromosome(m_pool->getMembers().at(0).chromosome);
}

PipePair Game::getClosestPipePair()
{
	int closestIndex = 0;
	float closestDist = INT_MAX;
	for (int i = 0; i < m_pipePairs.size(); ++i)
	{
		if (m_pipePairs.at(i).getX() <= closestDist && m_pipePairs.at(i).getX() > 250)
		{
			closestDist = m_pipePairs.at(i).getX();
			closestIndex = i;
		}
	}
	return m_pipePairs.at(closestIndex);
}
