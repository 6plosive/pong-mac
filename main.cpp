#include <bits/stdc++.h>
#include <raylib.h>
#define dbg(c) cout << #c << '=' << c << endl
using namespace std;

int clickedTargetState;
bool isballmoving = true;
bool ismoving = true;//without "PAUSED" message, use for animation
int state1Score = 0;
string state2Score = "0:0";
int state = 0;//0 = menu; 1 = single player
bool canChangeState = false;
bool shouldChangeState = false;
bool canStartCountdown = false;
int countdownCount = 0;
int state1BestScore = 0;
int state2Player1Score = 0;
int state2Player2Score = 0;
int state2WinScore = 5;
int countAnimation=60;
string state1BestScoreString = "Best Score : 0";
string state1FinalScoreString = "Score : 0";
string state2FinalScoreString = "Score : 0:0";

//provided by https://easings.net/
double easeOutBack(double x){
	return 3.70158 * pow(x - 1, 3) + 1.70158 * pow(x - 1, 2);
}
double easeOutExpo(double x){
    return (x==1)?1:1-pow(2, -10*x);
}
double easeOutQuart(double x){
    return 1-pow(1-x, 4);
}

Rectangle CreateRectangle(float x, float y, float width, float height){
    return {x-(width/2), y-(height/2), width, height};
}

void DrawRectangleLineRec(Rectangle rect, Color linecolor, Color color, float width){
    DrawRectangleRec(rect, linecolor);
    DrawRectangle(rect.x+width, rect.y+width, rect.width-(width*2), rect.height-(width*2), color);
}

struct Text{
    string text;
    int x, y;
    float fontsize;
    Color color;
    void draw(){
        DrawText(text.c_str(), x-(MeasureText(text.c_str(), fontsize)/2), y-(fontsize/2), fontsize, color);
    }
    void update(string __text){
        text=__text;
    }
};

void DrawCountdown(Text first, Text second, Text third, int baseFontsize){
    if(canStartCountdown){
        //dbg(countdownCount);
        //dbg(clickedTargetState);
        if(countdownCount>=0&&30>countdownCount){//0-29
            countdownCount++;
            //easeOutBack((countdownCount-1)/29);
            third.fontsize=baseFontsize*easeOutQuart((float)(countdownCount-1)/29);
            third.draw();
            if(clickedTargetState==2){
                DrawTriangle(Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2-10}, Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2+10}, Vector2{(float)GetScreenWidth()-50+(10*(float)sqrt(3)), (float)GetScreenHeight()/2}, Color{255, 255, 255, (unsigned char)(256*easeOutQuart((float)(countdownCount-1)/29)-1)});
            }
        }else if(countdownCount>=30&&60>countdownCount){//30-59
            countdownCount++;
            //easeOutQuart(1-((countdownCount-31)/29));
            third.fontsize=baseFontsize*easeOutQuart(1-((float)(countdownCount-31)/29));
            third.draw();
            if(clickedTargetState==2){
                DrawTriangle(Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2-10}, Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2+10}, Vector2{(float)GetScreenWidth()-50+(10*(float)sqrt(3)), (float)GetScreenHeight()/2}, Color{255, 255, 255, (unsigned char)(256*easeOutQuart(1-((float)(countdownCount-31)/29))-1)});
            }
        }else if(countdownCount>=60&&90>countdownCount){//60-89
            countdownCount++;
            //easeOutQuart((countdownCount-1)/29);
            second.fontsize=baseFontsize*easeOutQuart((float)(countdownCount-61)/29);
            second.draw();
            if(clickedTargetState==2){
                DrawTriangle(Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2-10}, Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2+10}, Vector2{(float)GetScreenWidth()-50+(10*(float)sqrt(3)), (float)GetScreenHeight()/2}, Color{255, 255, 255, (unsigned char)(256*easeOutQuart((float)(countdownCount-61)/29)-1)});
            }
        }else if(countdownCount>=90&&120>countdownCount){//90-119
            countdownCount++;
            //easeOutQuart(1-((countdownCount-31)/29));
            second.fontsize=baseFontsize*easeOutQuart(1-((float)(countdownCount-91)/29));
            second.draw();
            if(clickedTargetState==2){
                DrawTriangle(Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2-10}, Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2+10}, Vector2{(float)GetScreenWidth()-50+(10*(float)sqrt(3)), (float)GetScreenHeight()/2}, Color{255, 255, 255, (unsigned char)(256*easeOutQuart(1-((float)(countdownCount-91)/29))-1)});
            }
        }else if(countdownCount>=120&&150>countdownCount){//120-149
            countdownCount++;
            //easeOutQuart((countdownCount-1)/29);
            first.fontsize=baseFontsize*easeOutQuart((float)(countdownCount-121)/29);
            first.draw();
            if(clickedTargetState==2){
                DrawTriangle(Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2-10}, Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2+10}, Vector2{(float)GetScreenWidth()-50+(10*(float)sqrt(3)), (float)GetScreenHeight()/2}, Color{255, 255, 255, (unsigned char)(256*easeOutQuart((float)(countdownCount-121)/29)-1)});
            }
        }else if(countdownCount>=150&&180>countdownCount){//150-179
            countdownCount++;
            //easeOutQuart(1-((countdownCount-31)/29));
            first.fontsize=baseFontsize*easeOutQuart(1-((float)(countdownCount-151)/29));
            first.draw();
            if(clickedTargetState==2){
                DrawTriangle(Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2-10}, Vector2{(float)GetScreenWidth()-50, (float)GetScreenHeight()/2+10}, Vector2{(float)GetScreenWidth()-50+(10*(float)sqrt(3)), (float)GetScreenHeight()/2}, Color{255, 255, 255, (unsigned char)(256*easeOutQuart(1-((float)(countdownCount-151)/29))-1)});
            }
        }else if(countdownCount==180){
            countdownCount=0;
            canStartCountdown=false;
            ismoving=true;
            return;
        }
    }
}

class Balls;

class Button{
    private:
        bool isUnderMouse = false;
        Color linecolor;
        Color midcolor;
        Color fontcolor;
        Vector2 mousePos;
        int colorChangeSpeed;
        int targetState;
        //int countAnimation;
        void ColorModAdd(Color& color, int x){
            if(color.r<255){
                if(color.r+x>255){
                    color.r=255;
                }
                else{color.r+=x;}
            }
            if(color.g<255){
                if(color.g+x>255){
                    color.g=255;
                }
                else{color.g+=x;}
            }
            if(color.b<255){
                if(color.b+x>255){
                    color.b=255;
                }
                else{color.b+=x;}
            }
        }
        void ColorModDec(Color& color, int x){
        /*  linecolor.r-=5;
            linecolor.g-=5;
            linecolor.b-=5;*/
            if(color.r>0){
                if(color.r-x<0){
                    color.r=0;
                }
                else{color.r-=x;}
            }
            if(color.g>0){
                if(color.g-x<0){
                    color.g=0;
                }
                else{color.g-=x;}
            }
            if(color.b>0){
                if(color.b-x<0){
                    color.b=0;
                }
                else{color.b-=x;}
            }
        }
    public:
        Rectangle rect;
        string text;
        int fontsize;
        Button(Rectangle __rect, string __text, int __fontsize, int __targetState){
            rect = __rect;
            text = __text;
            fontsize = __fontsize;
            linecolor = WHITE;
            midcolor = BLACK;
            fontcolor = WHITE;
            colorChangeSpeed = 15;
            countAnimation=60;
            targetState = __targetState;
        };
        void draw(){
            DrawRectangleLineRec(rect, linecolor, midcolor, 5);
            DrawText(text.c_str(), rect.x+(rect.width/2)-(MeasureText(text.c_str(), fontsize)/2), rect.y+(rect.height/2)-(fontsize/2), fontsize, fontcolor);
            if(targetState==-1){
                DrawRectangleRec(CreateRectangle(rect.x+(rect.width/2)-10, rect.y+(rect.height/2), 10, rect.height-20), fontcolor);
                DrawRectangleRec(CreateRectangle(rect.x+(rect.width/2)+10, rect.y+(rect.height/2), 10, rect.height-20), fontcolor);
                
            }
        }
        void CheckMouse(Balls& ball);
        void DrawAnimation();
};

class Player{
    private:
    public:
        float sizeX=10,sizeY=100;
        float positionX, positionY;
        Player(int posX){
            positionX=posX;
            positionY=GetScreenHeight()/2;
        }
        void CheckOutOfBounds(int screenHeight){//USE AFTER DECLARED UNPROCESSED posY!!!
            if(positionY-(sizeY/2)<=0){
                positionY=sizeY/2;
                return;
            }
            if(positionY+(sizeY/2)>=screenHeight){
                positionY=screenHeight-(sizeY/2);
                return;
            }
        }
        void Draw(){
            DrawRectangle(positionX-(sizeX/2), positionY-(sizeY/2), sizeX, sizeY, WHITE);
        }
};

void DrawPauseMenu(Button restartButton, Button backToMenuButton, Text pausedText){
    DrawRectangleRec(CreateRectangle(GetScreenWidth()/2, 210, 300, 300), Color{255, 255, 255, 20});
    pausedText.draw();
    restartButton.draw();
    backToMenuButton.draw();
}

Player* p1ptr;
Player* p2ptr;

class Balls{
    private:
        float diameter = 10;
        float defaultspeedX = 2, defaultspeedY = 2;
        float LO = 0.7, HI = 1.3;
        Rectangle playerrect, ballrect;
    public:
        float positionX, positionY;
        float refspeedX, refspeedY;
        float speedX, speedY;
        Balls(){
            positionX = GetScreenWidth()/2;
            positionY = GetScreenHeight()/2;
            speedX=defaultspeedX;
            speedY=defaultspeedY;
            refspeedX=defaultspeedX;
            refspeedY=defaultspeedY;
        }
        void Update(){
            positionX+=speedX;
            positionY+=speedY;
        }
        void Draw(){
            DrawRectangle(positionX-(diameter/2), positionY-(diameter/2), diameter, diameter, WHITE);
        }
        void CheckWindowCollision(){
            //only check collision with Upper and lower since left and right are scores.
            if(positionY-diameter<=0 || positionY+diameter>=GetScreenHeight()){
                speedY=-speedY;
                refspeedY=-refspeedY;
            }
        }
        bool CheckPlayerCollision(Player player){
            playerrect = {player.positionX-(player.sizeX/2), player.positionY - (player.sizeY/2), player.sizeX, player.sizeY};
            ballrect = {positionX-(diameter/2), positionY-(diameter/2), diameter, diameter};
            //cout<<refspeedX<<" "<<refspeedY<<" "<<speedX<<" "<<speedY<<endl;
            if(CheckCollisionRecs(ballrect, playerrect)){
                speedX = -speedX;
                if(player.positionX<GetScreenWidth()/2){//left
                    positionX = player.positionX+(player.sizeX)+(diameter/2);
                }else{//right
                    positionX = player.positionX-(player.sizeX/2)-(diameter/2);
                }
                speedX *= 1.05;
                refspeedY *= 1.05;
                speedY= refspeedY*(LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))));
                //Template: float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
                //Source: https://stackoverflow.com/questions/686353/random-float-number-generation
                //score++;
                return true;
            }
            return false;
        }
        void reset(){
            positionX = GetScreenWidth()/2;
            positionY = GetScreenHeight()/2;
            speedX=defaultspeedX;
            speedY=defaultspeedY;
            refspeedX=defaultspeedX;
            refspeedY=defaultspeedY;
        }
};
void Button::CheckMouse(Balls& ball){//target state, eg: CheckMouse(0) will go to state 0 if clicked. Also include animation when hover on button.
    if(countAnimation!=60) return;
    mousePos = GetMousePosition();
    isUnderMouse = CheckCollisionPointRec(mousePos, rect);
    if(isUnderMouse && !(mousePos.x==400&&mousePos.y==225)){//default mouse pos
        //ColorModDec(linecolor, colorChangeSpeed);
        ColorModAdd(midcolor, colorChangeSpeed);
        ColorModDec(fontcolor, colorChangeSpeed);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            clickedTargetState=targetState;
            if(targetState<0){//functions, overlay, button without tp to other state
                if(targetState==-1){//pause button
                    isballmoving=!isballmoving;
                }
                if(targetState==-2){//quit, and save highest score
                    fstream scorestream;
                    scorestream.open("./score.PONG", std::ios::out | std::ios::trunc);
                    cout<<to_string(state1BestScore).c_str()<<endl;
                    scorestream<<to_string(state1BestScore).c_str();
                    scorestream.close();
                    CloseWindow();
                }
            }
            if(targetState>=0){//go to target state
                shouldChangeState=true;
                //state = targetState;
            }
            if(targetState==1){
                ball.reset();
                p1ptr->positionY=GetScreenHeight()/2;
                p2ptr->positionY=GetScreenHeight()/2;
                isballmoving=true;
                state1Score = 0;
            }
            if(targetState==2){
                ball.reset();
                //state2Score = "0:0";
                p1ptr->positionY=GetScreenHeight()/2;
                p2ptr->positionY=GetScreenHeight()/2;
                isballmoving=true;
                state2Player1Score = 0;
                state2Player2Score = 0;
            }
        }
    }else{            
        //ColorModAdd(linecolor, colorChangeSpeed);
        ColorModDec(midcolor, colorChangeSpeed);
        ColorModAdd(fontcolor, colorChangeSpeed);
    }
}
void Button::DrawAnimation(){//put it in the last line of each "if(state)" in draw, also need to put this function both in target state and button's state
    //dbg(countAnimation);
    if(targetState!=clickedTargetState) return;// Tips: If you can't figure some problem out, just chill out. Take a break and come back! :)
    if(shouldChangeState && countAnimation>0){// 1 to 60
        ismoving=false;
        /*fade out*/
        //DrawRectangleRec(CreateRectangle(GetScreenWidth()/2, GetScreenHeight()/2, GetScreenWidth(), GetScreenHeight()), Color{0, 0, 0, (unsigned char)(255/60*(60-countAnimation))});
        DrawRectangle(0, 0, GetScreenWidth(), ((GetScreenHeight())/90)*(60-countAnimation), BLACK);//top
        DrawRectangle(0, GetScreenHeight()-(((GetScreenHeight())/90)*(60-countAnimation)), GetScreenWidth(), ((GetScreenHeight())/90)*(60-countAnimation), BLACK);//bottom
        //DrawRectangle(0, 0, (GetScreenWidth()/90)*(60-countAnimation), GetScreenHeight(), BLACK);//left
        //DrawRectangle(GetScreenWidth()-((GetScreenWidth()/90)*(60-countAnimation)), 0, (GetScreenWidth()/90)*(60-countAnimation), GetScreenHeight(), BLACK);//right
        countAnimation--;
    }
    if(countAnimation==0){
        shouldChangeState=false;
        state=clickedTargetState;
        countAnimation--;
    }
    if(countAnimation>-60&&countAnimation<0){//-1 to -179
        /*fade in*/
        DrawRectangleRec(CreateRectangle(GetScreenWidth()/2, GetScreenHeight()/2, GetScreenWidth(), GetScreenHeight()), Color{0, 0, 0, (unsigned char)(255/60*(countAnimation))});
        countAnimation--;
    }
    if(countAnimation==-60){
        //ismoving=true;
        canStartCountdown=true;
        countAnimation=60;
    }
}

int main(){
	// Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    srand (time(NULL));

    InitWindow(screenWidth, screenHeight, "Pong - Made in MACBOOK!");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Player player1(15);
    Player player2(screenWidth-15);
    p1ptr =&player1;
    p2ptr =&player2;

    Balls ball;
    
    Vector2 mousePos = {0.0f, 0.0f};

    Button menuButton1(CreateRectangle(GetScreenWidth()/2, 175, 250, 75), "Singleplayer", 20, 1);
    Button menuButton2(CreateRectangle(GetScreenWidth()/2, 275, 250, 75), "Play With AI", 20, 2);
    Button menuButton3(CreateRectangle(GetScreenWidth()/2, 375, 250, 75), "Quit", 20, -2);
    Button backToMenuButton(CreateRectangle(GetScreenWidth()/2, 350, 250, 75), "Go Back To Menu", 20, 0);
    Button retryState1Button(CreateRectangle(GetScreenWidth()/2, 250, 250, 75), "Play again", 20, 1);
    Button retryState2Button(CreateRectangle(GetScreenWidth()/2, 250, 250, 75), "Play again", 20, 2);
    Button pauseButton(CreateRectangle(GetScreenWidth()/2, GetScreenHeight()-50, 50, 50), "", 0, -1);
    Button restartState1Button(CreateRectangle(GetScreenWidth()/2, 200, 200, 75), "Restart", 20, 1);
    Button restartState2Button(CreateRectangle(GetScreenWidth()/2, 200, 200, 75), "Restart", 20, 2);
    Button backToMenuButton2(CreateRectangle(GetScreenWidth()/2, 300, 200, 75), "Back To Menu", 20, 0);

    Text menuTitleText = {"Pong", GetScreenWidth()/2, 75, 100, WHITE};
    Text state1ScoreText = {to_string(state1Score), GetScreenWidth()/2, 30, 50, WHITE};
    Text state2ScoreText = {state2Score, GetScreenWidth()/2, 30, 50, WHITE};
    Text gameOverText = {"Game Over", GetScreenWidth()/2, 60, 75, WHITE};
    Text state1BestScoreText = {state1BestScoreString, GetScreenWidth()/2, 110, 20, WHITE};
    Text state1FinalScoreText = {state1FinalScoreString, GetScreenWidth()/2, 150, 50, WHITE};
    Text state4TitleText = {"You Lost!", GetScreenWidth()/2, 60, 75, WHITE};
    Text state2FinalScoreText = {state2FinalScoreString, GetScreenWidth()/2, 130, 40, WHITE};
    Text state5TitleText = {"You Won!", GetScreenWidth()/2, 60, 75, WHITE};
    Text pausedTitleText = {"Paused", GetScreenWidth()/2, 100, 70, WHITE};
    Text countdown1Text = {"1", GetScreenWidth()/2, GetScreenHeight()/2, 50, WHITE};
    Text countdown2Text = {"2", GetScreenWidth()/2, GetScreenHeight()/2, 50, WHITE};
    Text countdown3Text = {"3", GetScreenWidth()/2, GetScreenHeight()/2, 50, WHITE};

    fstream detectstream;
    detectstream.open("./score.PONG", std::fstream::app);
    detectstream.close();
    fstream readstream;
    string readtemp1="";
    readstream.open("./score.PONG", std::ios::in);
    char ch;
    while (1) {
        readstream >> ch;
        if (readstream.eof())
            break;
        readtemp1+=ch;
    }
    readstream.close();
    state1BestScore = (readtemp1=="")?0:stoi(readtemp1);
    ismoving=true;
    canStartCountdown=false;
    state=0;
    state1Score = 0;
    state2Score = "0:0";
    //state1BestScore = 0;//could store in files
    state2Player1Score = 0;
    state2Player2Score = 0;
    state2WinScore = 5;
    countAnimation=60;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        dbg(state1BestScore);
        //dbg(state);
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        mousePos=GetMousePosition();
        //dbg(r3);
        //cout<<"X: "<<mousePos.x<<" Y: "<<mousePos.y<<endl;
        if(state == 0){
            menuButton1.CheckMouse(ball);
            menuButton2.CheckMouse(ball);
            menuButton3.CheckMouse(ball);
        }
        if(state == 1){
            //cout<<"X: "<<ball.positionX<<" Y: "<<ball.positionY<<endl;
            pauseButton.CheckMouse(ball);
            restartState1Button.CheckMouse(ball);
            backToMenuButton2.CheckMouse(ball);
            /*if(IsKeyPressed(KEY_SPACE)){
                isballmoving=!isballmoving;
            }*/
            state1ScoreText.update(to_string(state1Score));
            if(isballmoving&&ismoving){
                player1.positionY=mousePos.y;
                player2.positionY=mousePos.y;
                player1.CheckOutOfBounds(screenHeight);
                player2.CheckOutOfBounds(screenHeight);
                ball.Update();
                ball.CheckWindowCollision();
                if(ball.CheckPlayerCollision(player1) || ball.CheckPlayerCollision(player2)){
                    state1Score++;
                }
                if(ball.positionX<0 || ball.positionX>GetScreenWidth()){//lost
                    state = 3;
                    state1BestScore = (state1BestScore>state1Score)?state1BestScore:state1Score;
                }
            }
        }
        if(state == 2){
            pauseButton.CheckMouse(ball);
            backToMenuButton2.CheckMouse(ball);
            restartState2Button.CheckMouse(ball);
            state2Score = to_string(state2Player1Score)+":"+to_string(state2Player2Score);
            state2ScoreText.update(state2Score);
            if(isballmoving&&ismoving){
                player2.positionY=mousePos.y;
                if(player1.positionY>ball.positionY){//ai module
                    player1.positionY-=abs(ball.speedX)*0.8;
                }else if(player1.positionY<ball.positionY){
                    player1.positionY+=abs(ball.speedX)*0.8;
                }
                player1.CheckOutOfBounds(screenHeight);
                player2.CheckOutOfBounds(screenHeight);
                //player1.positionY=mousePos.x;//dbg
                ball.Update();
                ball.CheckWindowCollision();
                ball.CheckPlayerCollision(player1);
                ball.CheckPlayerCollision(player2);
                if(ball.positionX<0){
                    state2Player2Score++;
                    ball.reset();
                }else if(ball.positionX>GetScreenWidth()){
                    state2Player1Score++;
                    ball.reset();
                }
                state2Score = to_string(state2Player1Score)+":"+to_string(state2Player2Score);
                if(state2Player1Score >=state2WinScore){//player1/Ai won
                    state = 4;
                }
                if(state2Player2Score >=state2WinScore){
                    state = 5;
                }
            }
        }
        if(state == 3){//state 1 game over, should show "Game over", score.
            backToMenuButton.CheckMouse(ball);
            retryState1Button.CheckMouse(ball);
            state1BestScoreString = "Best Score : " + to_string(state1BestScore);
            state1BestScoreText.update(state1BestScoreString);
            state1FinalScoreString = "Score : " + to_string(state1Score);
            state1FinalScoreText.update(state1FinalScoreString);
        }
        if(state == 4){//state 2 lost
            state2FinalScoreString = "Score : "+state2Score;
            state2FinalScoreText.update(state2FinalScoreString);
            backToMenuButton.CheckMouse(ball);
            retryState2Button.CheckMouse(ball);
        }
        if(state == 5){//state 2 won
            state2FinalScoreString = "Score : "+state2Score;
            state2FinalScoreText.update(state2FinalScoreString);
            backToMenuButton.CheckMouse(ball);
            retryState2Button.CheckMouse(ball);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);

            if(state == 0){//menu
                menuTitleText.draw();
                menuButton1.draw();
                menuButton2.draw();
                menuButton3.draw();
                menuButton1.DrawAnimation();//from
                menuButton2.DrawAnimation();//from
                backToMenuButton.DrawAnimation();//to
                backToMenuButton2.DrawAnimation();//to
            }
            if(state == 1){//singleplayer
                player1.Draw();
                player2.Draw();
                pauseButton.draw();
                ball.Draw();
                state1ScoreText.draw();
                DrawCountdown(countdown1Text, countdown2Text, countdown3Text, 150);
                if(!isballmoving&&ismoving){
                    //pausedTitleText.draw();
                    DrawPauseMenu(restartState1Button, backToMenuButton2, pausedTitleText);
                }
                backToMenuButton2.DrawAnimation();//from
                restartState1Button.DrawAnimation();//from and to
                menuButton1.DrawAnimation();//to
                retryState1Button.DrawAnimation();//to
            }
            if(state == 2){//singleplayer with bot
                state2ScoreText.draw();
                player1.Draw();
                player2.Draw();
                ball.Draw();
                pauseButton.draw();
                DrawCountdown(countdown1Text, countdown2Text, countdown3Text, 150);
                if(!isballmoving&&ismoving){
                    //pausedTitleText.draw();
                    DrawPauseMenu(restartState2Button, backToMenuButton2, pausedTitleText);
                }
                backToMenuButton2.DrawAnimation();//from
                restartState2Button.DrawAnimation();//from and to
                menuButton2.DrawAnimation();//to
                retryState2Button.DrawAnimation();//to
            }
            if(state == 3){
                gameOverText.draw();
                state1BestScoreText.draw();
                state1FinalScoreText.draw();
                backToMenuButton.draw();
                retryState1Button.draw();
                backToMenuButton.DrawAnimation();//from
                retryState1Button.DrawAnimation();//from
            }
            if(state == 4){//state 2 lost
                state4TitleText.draw();
                state2FinalScoreText.draw();
                backToMenuButton.draw();
                retryState2Button.draw();
                backToMenuButton.DrawAnimation();//from
                retryState2Button.DrawAnimation();//from
            }
            if(state == 5){//state 2 won
                state5TitleText.draw();
                state2FinalScoreText.draw();
                backToMenuButton.draw();
                retryState2Button.draw();
                backToMenuButton.DrawAnimation();//from
                retryState2Button.DrawAnimation();//from
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}