#include "Menu.h"


Menu::Menu(int winW, int winH) : winW(winW), winH(winH)
{
    isRunning = true;

    // Add Buttons Here
    float startX = winW / 2 - 100;
    float startY = winH / 2 - 50;
    float startWidth = 200;
    float startHeight = 100;

    startButton = Button("assets/button.png", startX, startY, startWidth, startHeight);
    addPlayerButton = Button("assets/button.png", startX - startWidth - 10, startY, startWidth, startHeight);
    removePlayerButton = Button("assets/button.png", startX + startWidth + 10, startY, startWidth, startHeight);


    // Add first player
    playerTextures.push_back(LoadTexture("assets/spriteSheet1.png"));
    playerPositions.push_back({(float)(winW), (float)(winH / 1.5)});
}

void Menu::Draw()
{   
    // Draw buttons
    startButton.Draw();
    addPlayerButton.Draw();
    removePlayerButton.Draw();

    // Draw players
    DrawPlayers();
}



void Menu::HandleButtonClicks(Context& context)
{
    // Handle Button clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        if (startButton.isClicked(mouseX, mouseY)) 
        {
            // Start Game
            context.menuSelection = MenuSelection::StartGame;
            isRunning = false;
        }

        else if (addPlayerButton.isClicked(mouseX, mouseY)) 
        {
            // Add Player
            AddPlayer(context);
        }

        else if (removePlayerButton.isClicked(mouseX, mouseY))
        {
            RemovePlayer(context);
        }

    }

}

void Menu::Update()
{
    UpdateCamera();
}


void Menu::AddPlayer(Context& context)
{
    float spaceBetweenPlayers = 150.0f;

    if (playerTextures.size() == 1) 
    {
        playerTextures.push_back(LoadTexture("assets/spriteSheet2.png"));
        playerPositions.push_back({playerPositions[0].x + spaceBetweenPlayers, playerPositions[0].y});

        context.numPlayers = 2;
    }

    else if (playerTextures.size() == 2)
    {
        playerTextures.push_back(LoadTexture("assets/spriteSheet3.png"));
        playerPositions.push_back({playerPositions[1].x + spaceBetweenPlayers, playerPositions[1].y});  

        context.numPlayers = 3;
    }
}

void Menu::RemovePlayer(Context& context)
{
    if (playerTextures.size() > 1) 
    {   
        UnloadTexture(playerTextures.back());
        playerTextures.pop_back();
        playerPositions.pop_back();

        context.numPlayers--;
    }
}


void Menu::DrawPlayers()
{
    for (int i = 0; i < playerPositions.size(); ++i)
    {   
        Rectangle sourceRec = {0.0f, 0.0f, (float)playerTextures[i].width / 4, (float)playerTextures[i].height};
        Rectangle destRec = {playerPositions[i].x, playerPositions[i].y, playerSize, playerSize};
        DrawTexturePro(playerTextures[i], sourceRec, destRec, {0, 0}, 0.0f, WHITE);
    }
}

void Menu::UpdateCamera()
{
    if (playerPositions.size() == 0) return;
    // Desired mid point
    int midX = winW / 2 - playerSize / 2;
    int midY = winH / 1.5;

    // get center point of all players
    int numPlayers = playerTextures.size();
    int avgX = 0;
    int avgY = 0;
    for (const FloatPoint& playerPos : playerPositions) 
    {
        avgX += playerPos.x;
        avgY += playerPos.y;
    }
    avgX /= numPlayers;
    avgY /= numPlayers;

    float lerpFactor = 0.05f;
    // move players
    for (FloatPoint& playerPos : playerPositions) 
    {
        float newPosX = playerPos.x + (midX - avgX) * lerpFactor;
        float newPosY = playerPos.y + (midY - avgY) * lerpFactor;

        playerPos.x = newPosX;
        playerPos.y = newPosY;
    }
}