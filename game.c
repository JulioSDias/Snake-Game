#define Tile 32

struct{
    int x, y;
    int DirX, DirY;
    int PrevX, PrevY;
    int Prev2X, Prev2Y;
}typedef HEAD;

HEAD Head = {2,2};

struct{
    int x, y;
}typedef FRUIT;

FRUIT Fruit = {0};

int TailSize = 0;
int *TailX, *TailY;
float Speed = 0.2f;

void Start();
void KeyPressed();

void Update();
void Eat();
void GenFruit();
void Collision();
void Score();
void End();

void Draw();
void Border(uint32 Color);

void Start(){
    TailX = malloc((sizeof *TailX) * TailSize);
    TailY = malloc((sizeof *TailY) * TailSize);
    srand(time(0));
    GenFruit();
}

void Update(){
    
    KeyPressed();
    Time.Elapsed += Time.Frame;
    if(Time.Elapsed >= Speed){
        
        
        Head.PrevX = Head.x;
        Head.PrevY = Head.y;
        
        Head.x += Head.DirX;
        Head.y += Head.DirY;
        
        if((Head.x == Fruit.x) && (Head.y == Fruit.y)) Eat();
        
        for(int i = 1; i < TailSize; i++){
            Head.Prev2X = TailX[i];
            Head.Prev2Y = TailY[i];
            TailX[i] = Head.PrevX;
            TailY[i] = Head.PrevY;
            Head.PrevX = Head.Prev2X;
            Head.PrevY = Head.Prev2Y;
        }
        
        /*for(int i = TailSize; i >= 1; i--){
            TailX[i] = TailX[i - 1];
            TailY[i] = TailY[i - 1];
        }*/
        
        TailX[0] = Head.PrevX;
        TailY[0] = Head.PrevY;
        
        Collision();
        
        Time.Elapsed = 0;
    }
    
}

void GenFruit(){
    do{
        Fruit.x = rand() % (RenderBuffer.Width/Tile);
        Fruit.y = rand() % (RenderBuffer.Height/Tile);
    }while(Fruit.x == 0 || Fruit.x >=  RenderBuffer.Width/Tile - 2 || Fruit.y == 0 || Fruit.y >= RenderBuffer.Height/Tile - 2 || Fruit.x == Head.x || Fruit.y == Head.y);
}

void Collision(){
    if(Head.x == 0) Head.x = RenderBuffer.Width/Tile - 2;
    if(Head.y == 0) Head.y = RenderBuffer.Height/Tile - 2;
    if(Head.x == RenderBuffer.Width/Tile - 1) Head.x = 1;
    if(Head.y == RenderBuffer.Height/Tile - 1) Head.y = 1;
    
    for(int i = 0; i < TailSize; i++){
        if(Head.x == TailX[i] && Head.y == TailY[i]) End(); 
    }
}

void End(){
    free(TailX);
    free(TailY);
    Running = 0;
}

void Eat(){
    TailSize++;
    int *NewTailX = realloc(TailX, (sizeof *NewTailX) * TailSize);
    int *NewTailY = realloc(TailY, (sizeof *NewTailY) * TailSize);
    if(NewTailX == 0 && NewTailY == 0){
        printf("Realloc Failed");
    }else{
        TailX = NewTailX;
        TailY = NewTailY;
    }
    
    Speed *= 0.95f;
    GenFruit();
}

void Draw(){
    ColorScreen(PACIFIC_BLUE);
    Border(CG_BLUE);
    Score();
    
    /*Fruit*/
    DrawRect(Fruit.x * Tile, Fruit.y * Tile, Tile, Tile, RED);
    
    /*Head*/
    DrawRect(Head.x * Tile, Head.y * Tile, Tile, Tile, ORANGE);
    
    /*Tail*/
    for(int i = 0; i < TailSize; i++)
        DrawRect(TailX[i] * Tile, TailY[i] * Tile, Tile, Tile, ORANGE);
}


void KeyPressed(){
    if(pressed(LEFT) && Head.DirX != 1){ 
        Head.DirX = -1;
        Head.DirY = 0;
    }
    if(pressed(RIGHT) && Head.DirX != -1){
        Head.DirX = 1;
        Head.DirY = 0;
    }
    if(pressed(UP) && Head.DirY != 1){
        Head.DirY = -1;
        Head.DirX = 0;
    }
    if(pressed(DOWN) && Head.DirY != -1){ 
        Head.DirY = 1;
        Head.DirX = 0;
    }
    
    Head.x = Clamp(Head.x, 0, RenderBuffer.Width/Tile);
    Head.y = Clamp(Head.y, 0, RenderBuffer.Height/Tile);
}

void Border(uint32 Color){
    DrawRect(0, 0, RenderBuffer.Height, Tile, Color);
    DrawRect(0, 0, Tile, RenderBuffer.Width, Color);
    DrawRect(0, RenderBuffer.Height - Tile, Tile, RenderBuffer.Width, Color);
    DrawRect(RenderBuffer.Width - Tile, 0, RenderBuffer.Height, Tile, Color);
}

void Score(){
    int Decimal, Unit;
    if(TailSize < 10) DrawNumber(1 * Tile, 0, TailSize);
    if(TailSize >= 10){
        Decimal = TailSize/10;
        Unit = TailSize%10;
        DrawNumber(0, 0, Decimal);
        DrawNumber(1 * Tile, 0, Unit);
    }
}