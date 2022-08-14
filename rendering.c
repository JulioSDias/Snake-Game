void ColorScreen(uint32 color);
void DrawRect(int x0, int y0, int height, int width, uint32 color);
void DrawSprite(int x0, int y0, uint32 color, int TileMap[8][8]);
void DrawChess(uint32 color1, uint32 color2);


void ColorScreen(uint32 color){
    uint32 *pixel = RenderBuffer.Pixels;
    int y, x;
    for(y = 0; y < RenderBuffer.Height; y++){
        for(x = 0; x < RenderBuffer.Width; x++){
            *pixel = color;
            pixel++;
        }
    }
    
}

void DrawRect(int x0, int y0, int height, int width, uint32 color){
    
    x0 = Clamp(x0, 0, RenderBuffer.Width - width);
    y0 = Clamp(y0, 0, RenderBuffer.Height - height);
    
    int y, x;
    for(y = y0; y < y0 + height; y++){
        uint32 *pixel =  RenderBuffer.Pixels + x0 + RenderBuffer.Width * y;
        for(x = x0; x < x0 + width; x++){
            *pixel = color;
            pixel++;
        }
    }
}

void DrawSprite(int x0, int y0, uint32 color, int TileMap[8][8]){
    
    int size = 4;
    
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(TileMap[i][j] == 0)
                DrawRect(x0 + j * size, y0 + i * size, size, size, CG_BLUE);
            if(TileMap[i][j] == 1)
                DrawRect(x0 + j * size, y0 + i * size, size, size, color);
        }
    }
    
}

void DrawNumber(int x0, int y0, int var){
    switch(var){
        case 0:
        DrawSprite(x0, y0, WHITE, Num0);
        break;
        case 1:
        DrawSprite(x0, y0, WHITE, Num1);
        break;
        case 2:
        DrawSprite(x0, y0, WHITE, Num2);
        break;
        case 3:
        DrawSprite(x0, y0, WHITE, Num3);
        break;
        case 4:
        DrawSprite(x0, y0, WHITE, Num4);
        break;
        case 5:
        DrawSprite(x0, y0, WHITE, Num5);
        break;
        case 6:
        DrawSprite(x0, y0, WHITE, Num6);
        break;
        case 7:
        DrawSprite(x0, y0, WHITE, Num7);
        break;
        case 8:
        DrawSprite(x0, y0, WHITE, Num8);
        break;
        case 9:
        DrawSprite(x0, y0, WHITE, Num9);
        break;
    }
}

void DrawChess(uint32 color1, uint32 color2){
	int SIZE = 32;
    int y, x;
    for(y = 0; y < RenderBuffer.Height/SIZE; y++){
		for(x = 0; x < RenderBuffer.Width/SIZE; x++){
			if(y%2 == 0){
				if (x%2 == 0){
					DrawRect(x * SIZE , y * SIZE, SIZE, SIZE, color1);	
				} else {
					DrawRect(x * SIZE , y * SIZE, SIZE, SIZE, color2);	
                }
			} else {
				if (x%2 == 0){
                    DrawRect(x * SIZE , y * SIZE, SIZE, SIZE, color2);	
				} else {
                    DrawRect(x * SIZE , y * SIZE, SIZE, SIZE, color1);	
				}
			}	
		}
	}
}


