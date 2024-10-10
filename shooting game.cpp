//made by Sergio Vargas Vicente
#include<iostream>
#include<Windows.h>
#include<time.h>
#include<iostream>
#include<stdlib.h>
#include<stdio.h> 
#include<conio.h>
#include<vector>
#include <cstdlib> 
#include <ctime>  
using namespace std;
int end = 0;
int screen[2];
//character = \xCE
int charpos[]={10,10};
string tilefloor = " ";// \xFA   //este es el tile del suelo
string tilebala="o";//\x91 //este es el tile de la bala
string tilechar="^";
int frame=0;
int direccion=8;
int delay=0;
int ronda=1;
int pausa=0;
int nenemigos=0;
int spawncap=1;
int enemieskilled=0;
int fase2=16,fase2done=true,fase3done=true;
int baladisparada;
int maxspawn=5;
struct punto {
    int x;
    int y;
};

//numeroAleatorio=min+rand()%(max-min+1)

//vector <vector<int> > mapa;
int setpos(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

int setcolor(WORD color) { //CAMBIAR COLOR	
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, color);
}
/* //terminal colors
    BLACK             = 0,
    DARKBLUE          = FOREGROUND_BLUE,
    DARKGREEN         = FOREGROUND_GREEN,
    DARKCYAN          = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED           = FOREGROUND_RED,
    DARKMAGENTA       = FOREGROUND_RED | FOREGROUND_BLUE,
    DARKYELLOW        = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY          = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY              = FOREGROUND_INTENSITY,
    BLUE              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN              = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED               = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA           = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
*/

void hidecursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false; // Establece la visibilidad del cursor a falso
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void calcularlinea(int x0, int y0, int x1, int y1, vector<punto>& _vector) {
    int dx = abs(x1 - x0); //diferencia entre los dos puntos (ej: 10-5 es igual a 5 puntos entremedio)
    int dy = abs(y1 - y0); //diferencia entre los dos puntos
    int sx = (x0 < x1) ? 1 : -1; //saber si hay que sumar o restar x, este valor siempre sera 1 o -1
    int sy = (y0 < y1) ? 1 : -1;//saber si hay que sumar o restar y, este valor siempre sera 1 o -1
    int err = dx - dy; //error acumulado, si la linea calculada se sale mucho de la linea dibujada, este numero aumentará    
	_vector.clear();	
	while (true) 
	{
       /* setpos(x0,y0);  //esto es para mostrar la linea dibujada
    	cout<<".";*/
        _vector.push_back({x0, y0});

        if (x0 == x1 && y0 == y1) break;//cuando puntos es igual a pos se acaba el bucle
        int e2 = 2 * err;

        if (e2 > -dy) {//ava
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
	}
		
}



//clase para el enemigo
class enemigo{
    private: //stats, only own methods can acces this
	int x;
	int y;
	int id;
	
	public: //methods
	vector<punto> path;
	enemigo (int,int,int);
	void move(int,int);
	//void pathfind(int,int);

	int getx(){return x;};
	int gety(){return y;};
	int getid(){return id;};
	
};

enemigo::enemigo(int _x,int _y,int _id)//constructor del objeto
{
	x=_x;
	y=_y;
	id=_id;
}

void enemigo::move(int _x, int _y){
	x=_x;
	y=_y;	
}
//vector de enemigos
vector<enemigo>listaenemigos;



//clase para las balas
class bala{
	private://own stats
	int x;
	int y;
	int dir;
	public://methods
	bala(int,int,int);
	void move(int,int);
	int getx(){return x;};
	int gety(){return y;};
	int getdir(){return dir;};
};

bala::bala(int _x,int _y,int _dir)//constructor del objeto
{
	x=_x;
	y=_y;
	dir=_dir;
}

void bala::move(int _x, int _y){
	x=_x;
	y=_y;
}

//vector de balas
vector<bala>listabalas;

int dibujarpersonajes()
{
	
	//bala
	for(int i=0;i<listabalas.size();i++)
	{
		setpos(listabalas[i].getx(),listabalas[i].gety());
		setcolor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		cout<<tilebala;
		setcolor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	//protagonista
	
	switch(direccion) //el protagonista cambia depende de donde mire
	{
    case 8: 
        tilechar= "^";
        break;
    case 2: 
        tilechar= "v";
        break;
    case 4:
        tilechar= "<";
        break;
    case 6: 
        tilechar=">";
        break;
	}
	setpos(charpos[0],charpos[1]);
	setcolor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout<<tilechar; //
	setcolor(
	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE 
	);
	

 char colores[] = { //color aleatorio
 	
    FOREGROUND_BLUE,
    FOREGROUND_GREEN,
    FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED,
    FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
   
    FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_RED,
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        
    };
	
	//enemigos
	for(int i=0;i<listaenemigos.size();i++)
	{
		setpos(listaenemigos[i].getx(),listaenemigos[i].gety());
		if(listaenemigos[i].getid()%10==0 or listaenemigos[i].getid()>fase2)//boss
		{
			setcolor(colores[1+rand()%(14-1+1)]);
			cout<<"\xCE";
		}
		
		else{
			setcolor(colores[ listaenemigos[i].getid()%14]);//peón
			cout<<"\xF6";
		}
		
		
		
		
		setcolor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE  );
	}

}


int dibujarescenario()
{
//dibujar escenario 
	setcolor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    for(int i=0;i<=screen[1]-1;i++)//y
    {
    	for(int k=0;k<=screen[0]-1;k++)//x
    	{
			//dibujar escenario
    		//arriba
			if(i==0 and k==0)
    		{
				setpos(k,i);
				cout<<"\xC9";
			}else if(i==0 and k==screen[0]-1)//arriba derecha
			{
				setpos(k,i);
				cout<<"\xBB";
			}else if(i==0)//pared arriba
			{
				setpos(k,i);
				cout<<"\xCD";
			}
			else if(i==screen[1]-1 and k==0) //abajo izquierda
			{
				setpos(k,i);
				cout<<"\xC8";
			}else if (i==screen[1]-1 and k==screen[0]-1 ) //abajo derecha
			{
				setpos(k,i);
				cout<<"\xBC";
			}else if( i!=screen[1]-1 and k==0 or k==screen[0]-1 )//paredes izquierda y derecha
			{
				setpos(k,i);
				cout<<"\xBA";
				
			}else if(i==screen[1]-1)//pared abajo
			{
				setpos(k,i);
				cout<<"\xCD";
			}else{ //dibujar el suelo
				setpos(k,i);
				cout<<tilefloor;
			}
			
    	}
    }
    setpos(0,screen[1]/2-1);cout<<"\xBC";
    setpos(0,screen[1]/2);cout<<tilefloor; //boquete izquierdo
    setpos(0,screen[1]/2+1);cout<<"\xBB";
    
    setpos(screen[0]-1,screen[1]/2-1);cout<<"\xC8";
    setpos(screen[0]-1,screen[1]/2);cout<<tilefloor; //boquete derecho
    setpos(screen[0]-1,screen[1]/2+1);cout<<"\xC9";
    
    setpos(screen[0]/2+1,0);cout<<"\xC8";
    setpos(screen[0]/2,0);cout<<tilefloor; //boeuqte arriba
    setpos(screen[0]/2-1,0);cout<<"\xBC";
    
    setpos(screen[0]/2+1,screen[1]-1);cout<<"\xC9";
    setpos(screen[0]/2,screen[1]-1);cout<<tilefloor; //boeuqte abajo
    setpos(screen[0]/2-1,screen[1]-1);cout<<"\xBB";
}



int logica(){

	/* //para mover el enemigo
	setpos(listaenemigos[0].getx(), listaenemigos[0].gety());
	cout<<tilefloor;
	listaenemigos[0].move(10,5);	
	dibujarpersonajes();
	*/
	
	/* //crear enemigos
	enemigo nuevo(X,Y,ID); //crear enemigo
	listaenemigos.push_back(nuevo); //añadir enemigo al vector publico*/
	
	//mecanica spawn enemigos
	if (spawncap>maxspawn){spawncap=maxspawn;}//mantener el spawn de enemigos en maximo 5
	if (listaenemigos.size()<=spawncap)
	{
		nenemigos++; //para las id's
		int randomboquete= 1+rand()%(4-1+1);//el lugar de spawneo sera aleatorio
		enemigo nuevo(0, 0, 0); // poner esto aqui fuera preve un error que no se que es
		switch (randomboquete)
		{
		
			case 1: //spawnear enemigo en boquete izquierdo
			 nuevo= enemigo(0,screen[1]/2,nenemigos); 
			listaenemigos.push_back(nuevo); 	
			break;
			
			case 2:
			nuevo=enemigo(screen[0]-1,screen[1]/2,nenemigos); 
			listaenemigos.push_back(nuevo); 	
			break;
			
			case 3:
			nuevo=enemigo(screen[0]/2,0,nenemigos); 
			listaenemigos.push_back(nuevo); 
			break;
			case 4:
			nuevo=enemigo(screen[0]/2,screen[1]-1,nenemigos); 
			listaenemigos.push_back(nuevo); 
			break;
		}
		
	}
	
	
	
	
	//delay balas
	delay--;
	
	//mover balas
	for(int i=0;i<listabalas.size();i++)
	{
		switch(listabalas[i].getdir())
		{
		
			case 8: 
			setpos(listabalas[i].getx(),listabalas[i].gety());
			cout<<tilefloor;
			listabalas[i].move(listabalas[i].getx(),listabalas[i].gety()-1);
			break;
		
			case 2: 
			setpos(listabalas[i].getx(),listabalas[i].gety());
			cout<<tilefloor;
			listabalas[i].move(listabalas[i].getx(),listabalas[i].gety()+1);
			break;
			
			case 4: 
			setpos(listabalas[i].getx(),listabalas[i].gety());
			cout<<tilefloor;
			listabalas[i].move(listabalas[i].getx()-1,listabalas[i].gety());
			break;
			
			case 6: 
			setpos(listabalas[i].getx(),listabalas[i].gety());
			cout<<tilefloor;
			listabalas[i].move(listabalas[i].getx()+1,listabalas[i].gety());
			break;
		
		
		}
	}
	
	//colision balas pared
	for(int i=0;i<listabalas.size();i++)
	{
		if(listabalas[i].getx()<=0 or listabalas[i].getx()>=screen[0]-1 or listabalas[i].gety()<=0 or listabalas[i].gety()>=screen[1]-1)
		{
			setpos(listabalas[i].getx(),listabalas[i].gety());
			listabalas.erase(listabalas.begin()+i);
		}
	}
	
	
	
	//colisio balas enemigo
	int borrarbala=true;
	for(int i=0;i<listabalas.size();i++)
	{
		for(int k=0;k<listaenemigos.size();k++)
		{
		
			if(listabalas[i].getx()==listaenemigos[k].getx() and listabalas[i].gety()==listaenemigos[k].gety())
			{
				setpos(listabalas[i].getx(),listabalas[i].gety());
				cout<<tilefloor;
				listaenemigos.erase(listaenemigos.begin()+k);
				if(borrarbala==true)
				{
				listabalas.erase(listabalas.begin()+k);
				borrarbala=false;
				}
				
				spawncap++;
				enemieskilled++;
			}
		}
	}
	
	
	
	
	//mover enemigos
	
	for(int i=0;i<listaenemigos.size();i++)
	{
		if(!listaenemigos[i].path.empty() and frame%2==0) //el enemigo se mueve cada 2 frames
		{
			int random = 0+rand()%(2-0+0);//velocidad random 
			setpos(listaenemigos[i].getx(),listaenemigos[i].gety());
			cout<<tilefloor;
			
			if(listaenemigos[i].getid()%10==0 or listaenemigos[i].getid()>fase2 ) //movimiento bosses
			{
			listaenemigos[i].move( listaenemigos[i].path[1+rand()%(3-1+0)].x, listaenemigos[i].path[1+rand()%(3-1+0)].y );
			}
			else{
			listaenemigos[i].move( listaenemigos[i].path[random].x, listaenemigos[i].path[random].y ); //movimiento peones
			}
			
			listaenemigos[i].path.erase(listaenemigos[i].path.begin()); //borrar el ultimo punto
			//dibujarpersonajes(); //desactivado pq causa lag creo
		}	
		
		 if(frame%4==0)//calcular pathfinding
		{
			calcularlinea(listaenemigos[i].getx(),listaenemigos[i].gety(),charpos[0],charpos[1],listaenemigos[i].path);
		}
	}
	
	
	
	//mostrar datos en la esquina
/*	setpos(2,0);
	cout<< "En:" << listaenemigos.size() << ",ID:" << nenemigos;*/
	//mostrar ronda en la esquina
	setpos(screen[0]-6,screen[1]-1);
	cout<<enemieskilled;

	
	//colision con enemigos = se acaba el juego
	for(int i=0;i<listaenemigos.size();i++)
	{
		if( charpos[0] == listaenemigos[i].getx() and charpos[1] == listaenemigos[i].gety() )
		{
			end=1;
		}
	}
	
	
	//pasar a la fase2
	if(enemieskilled>=fase2 and fase2done==true)
	{
		
		//ahora resetear el contador de spawncap
		spawncap=0;
		fase2done=false;
	}
	//fase 3, modificar maxspawncap a 10
	if(enemieskilled>=30 and fase3done==true)
	{
		maxspawn=10;
		fase3done=false;
	}
	if(enemieskilled>=35)
	{
		maxspawn=20;
	}
		if(enemieskilled>=40)
	{
		maxspawn=80;
	}

	//bresenham  NO EN USO, ESTA MAS ARRIBA
	/*if (frame==0)
	{
		//calcular linea
		
	//	calcularlinea(listaenemigos[0].getx(),listaenemigos[0].gety(),charpos[0],charpos[1],listaenemigos[0].path);//deberia guardarse en el vector1
		
		//mostrar linea abajo
		setpos(0,30);
    	for(int i=0;i<vector1.size();i++)
    	{
    			cout<<"("<<vector1[i].x<<","<<vector1[i].y<<")"<<endl;	
    	}
	
		//listaenemigos[0].pathfind(charpos[0],charpos[1]);
	}*/

}


void pausa1()
{
	int pausa=1;
	setpos(screen[0]/2-3,screen[1]/2);
	cout<<"Pause";
	Sleep(500);
	
		while(pausa==1)
		{	
			  if (GetAsyncKeyState('N') & 0x8000 or GetAsyncKeyState('Z') & 0x8000)
	   		 {
	            pausa=0;
	   		 }
	   		  if (GetAsyncKeyState('B') & 0x8000 )
	   		 {
	            pausa=0;
	            end=true;
	   		 }
		}
	Sleep(300);	
	setpos(screen[0]/2-3,screen[1]/2);
	cout<<tilefloor<<tilefloor<<tilefloor<<tilefloor<<tilefloor;		
}

void input()
{
 
 	
   
	    if (GetAsyncKeyState('I') & 0x8000 or GetAsyncKeyState('W') & 0x8000) //arriba
	    {
	    	
	        if (charpos[1] > 1) //colision
	        {
	        	direccion=8;
	            setpos(charpos[0], charpos[1]);
	            cout << tilefloor;
	            charpos[1]--;
	            dibujarpersonajes();
	        }
	    }
	    
	    if (GetAsyncKeyState('K') & 0x8000 or GetAsyncKeyState('S') & 0x8000) //abajo
	    {
	    	
	        if (charpos[1] < screen[1] - 2) //colision 
	        {
	        	direccion=2;
	            setpos(charpos[0], charpos[1]);
	            cout << tilefloor;
	            charpos[1]++;
	            dibujarpersonajes();
	        }
	    }
	    
	    if (GetAsyncKeyState('J') & 0x8000 or GetAsyncKeyState('A') & 0x8000) //izquierda
	    {
	    	
	        if (charpos[0] > 1) //colision
	        {
	        	direccion=4;
	            setpos(charpos[0], charpos[1]);
	            cout << tilefloor;
	            charpos[0]--;
	            dibujarpersonajes();
	        }
	    }
	    
	    if (GetAsyncKeyState('L') & 0x8000 or GetAsyncKeyState('D') & 0x8000) //derecha
	    {
	    
	        if (charpos[0]<screen[0]-2) //colision
	        {
	        	direccion=6;
	            setpos(charpos[0], charpos[1]);
	            cout << tilefloor;
	            charpos[0]++;
	            dibujarpersonajes();
	        }
    	
	}
	

	
    
    //pausa
       if (GetAsyncKeyState('N') & 0x8000 or GetAsyncKeyState('Z') & 0x8000) 
    {
        pausa1();
    }
    baladisparada=false;
    if (baladisparada==false and delay<1 and (delay<=0 and GetAsyncKeyState(VK_LBUTTON) & 0x8000 or GetAsyncKeyState('M') & 0x8000   and delay<=0 or GetAsyncKeyState('X') & 0x8000 and delay<=0) ) //DISPARAR CON CLICK IZQUIERDO o tecla M
    {
		bala nueva(charpos[0],charpos[1],direccion);
        listabalas.push_back(nueva);
        baladisparada=true;
        delay=4;
    }
    
    
    if (GetAsyncKeyState('B') & 0x8000) //tecla para cerrar el juego
    {
        end=1;
    }
}

int main() {
	srand(time(NULL));//Aqui se empieza un reloj para hacer numeros aleatorios	
	//tamaño ventana
	    SMALL_RECT rect;
	    COORD coord;
	    coord.X = 20; // Define X
	    coord.Y = 20; // Define Y para el tamaño del búfer.
	
	    rect.Top = 0;
	    rect.Left = 0;
	    rect.Bottom = coord.Y-1; // Altura para ventana
	    rect.Right = coord.X-1; // Ancho para ventana
	
	    HANDLE hwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleScreenBufferSize(hwnd, coord); // Establece tamaño del búfer
	    SetConsoleWindowInfo(hwnd, TRUE, &rect); // Establece el tamaño de la ventana
	    SetConsoleTitle("");
    	hidecursor();
 	    
 	     //Esta maravilla convierte el ratón a un input
		 HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE); 
	    DWORD mode;
	    GetConsoleMode(hConsole, &mode);
	    SetConsoleMode(hConsole, mode & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_MOUSE_INPUT));
 		
 		
    screen[0] = coord.X; 
    screen[1] = coord.Y;
   
   
    /*//aun no tiene uso todo esto, nisiquiera se si funciona
mapa.resize(screen[0], vector<int>(screen[1], 0)); 
   for(int i=0;i<=screen[1]-1;i++) //y 
{
    for(int k=0;k<=screen[0]-1;k++) //x
    {
        if (i == 0 || i == screen[1] - 1 || k == 0 || k == screen[0] - 1) {
            mapa[k][i]=1;//registra los lados como pareces
        } else {
            mapa[k][i] = 0; // registra todo el resto como suelo
        }
        
    }
}*/
    
  	//dibujarescenario
  	dibujarescenario();
	//aqui empieza el juego
	
	
    while (!end) 
	{
	
	input();
	logica();
	dibujarpersonajes();
	Sleep(90);
	frame++;
	if(frame%5==0){setpos(0,0);}
    }
    
    
    setpos(screen[0]/2-6,screen[1]/2);
    cout<<"-Game Ended-";
    setpos(0,screen[1]-1);
    system("pause");
    
    return 0;
}
