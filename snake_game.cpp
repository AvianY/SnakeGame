#include<ncurses.h>
#include<vector>
#include<iostream>
#include<cstdlib>
#include<ctime>

const int Dolzina = 40;
const int Visina = 20;

void narisi_rob();
void narisi_kaco( std::vector< std::vector<int>> &);
void narisi_sadje( std::vector< int > &);
enum Smer { gor, dol, levo, desno };
Smer pridobi_smer(int, std::vector< std::vector<int>> &);
Smer nova_smer( char input, Smer smer);
void premakni_kaco( Smer, std::vector< std::vector<int>> &);
void podaljsaj_kaco( Smer, std::vector< std::vector<int>> &);
bool dotik_kace( std::vector< std::vector<int>> &);
bool dotik_roba( std::vector< std::vector<int>> &);
bool dotik_sadja( std::vector< int > &, std::vector< std::vector< int>> &);
std::vector<int> novo_sadje( std::vector< std::vector<int>> &);


int main()
{

	/* if (has_colors() == FALSE) { */
	/* 	endwin(); */
	/* 	printf("Your terminal does not support color\n"); */
	/* 	exit(1); */
	/* } */
	/* start_color(); */
	/* init_pair(1, COLOR_WHITE, COLOR_BLACK ); */
	/* init_pair(2, COLOR_GREEN, COLOR_BLACK ); */

	char input;
	std::vector< std::vector<int>> kaca = { {2, 2}, {3, 2}, {4, 2}  };
	std::vector< int > sadje = { 5, 5 };
	bool OVER = false;
	Smer smer = dol;

	initscr(); // inicializacija ncurses-a ?
	cbreak();
	curs_set(0); // Skrije kursor
	noecho(); // Ne izpisuje vtipkanih znakov
	nodelay(stdscr, TRUE);
	narisi_rob();
	narisi_kaco(kaca);
	refresh();

	while( true )
	{
		std::clock_t begin = std::clock();
		input = getch();
		flushinp();

		if( input == 'q' || OVER )
		{
			move( 5, 5 );
			printw( "GAME OVER" );
			refresh();
			getchar();
			break;
		}

		smer = nova_smer(input, smer);
		if ( dotik_sadja(sadje, kaca) ){
			sadje = novo_sadje(kaca);
			podaljsaj_kaco(smer, kaca);
		} else {
			premakni_kaco(smer, kaca);
		}
		erase();
		narisi_sadje(sadje);
		narisi_rob();
		narisi_kaco(kaca);
		refresh();
		OVER = dotik_kace(kaca) | dotik_roba(kaca);
		while( std::clock() - begin < 100000 ){} 
	}

	endwin();
	return 0;
}

void narisi_rob()
{
	// Narišemo rob območja igre
	for( int i = 0 ; i < Dolzina + 1 ; i++ )
	
	{
		move(0,i);
		printw("#");
		move(Visina,i);
		printw("#");
	}
	for( int i = 1 ; i < Visina ; i++ )
	
	{
		move(i,0);
		printw("#");
		move(i,Dolzina);
		printw("#");
	}
}

void narisi_kaco( std::vector< std::vector<int>>& kaca )
{
	/* attron( COLOR_PAIR(2) ); */
	// Narisemo kaco
	int size = kaca.size();
	for ( int i = 0; i < size-1; i++)
	{
		move( kaca[i][0], kaca[i][1] );

		switch( pridobi_smer( i, kaca ) )
		{
				case gor:   printw("A"); break;
				case dol:   printw("V"); break; 
				case levo: 	printw("<"); break; 
				case desno:	printw(">"); break; 
		}
	}
	// Ko je ves rep narisan, dokoncamo z glavo
	move(kaca[size-1][0], kaca[size-1][1]);
	printw("O");
	/* attroff( COLOR_PAIR(2) ); */
}

Smer pridobi_smer( int i, std::vector< std::vector<int>> & kaca)
{
	int dy = kaca[i+1][0] - kaca[i][0];
	int dx = kaca[i+1][1] - kaca[i][1];

	if ( dy == -1 && dx == 0 ) return gor;
	if ( dy == 1 && dx == 0 ) return dol;
	if ( dy == 0 && dx == -1 ) return levo;
	if ( dy == 0 && dx == 1 ) return desno;
	std::cout << "Neki je hudo narobe: " << dy << ", " << dx;
	std::exit(1);
}

Smer nova_smer( char input, Smer smer )
{
	if(   input == 'h' && smer != desno ) return levo;
	else if( input == 'j' && smer != gor ) return dol;
	else if( input == 'k' && smer != dol ) return gor;
	else if( input == 'l' && smer != levo ) return desno;
	else return smer;

	/* switch(input) */
	/* { */
	/* 	case 'h': if( smer != desno ) return levo; */
	/* 	case 'j': if( smer != gor )   return dol; */
	/* 	case 'k': if( smer != dol )   return gor; */
	/* 	case 'l': if( smer != levo )  return desno; */
	/* 	default: return smer; */
	/* } */
}

void premakni_kaco( Smer smer, std::vector< std::vector<int>> & kaca)
{
	kaca.erase(kaca.begin());
	int size = kaca.size();
	int head_x = kaca[size - 1][1];
	int head_y = kaca[size - 1][0];
	switch(smer)
	{
		case gor: kaca.push_back({head_y - 1, head_x}); break;
		case dol: kaca.push_back({head_y + 1, head_x}); break;
		case levo: kaca.push_back({head_y, head_x - 1}); break;
		case desno: kaca.push_back({head_y, head_x + 1}); break;
	}
	return;
}

bool dotik_kace( std::vector< std::vector<int>> & kaca )
{
	int size = kaca.size();
	int head_x = kaca[size - 1][1];
	int head_y = kaca[size - 1][0];

	for ( unsigned int i = 0; i < kaca.size() - 1; i++ )
	{
		if( head_x == kaca[i][1] && head_y == kaca[i][0] ) return true;
	}
	return false;
}

bool dotik_roba( std::vector< std::vector<int>> & kaca )
{
	int size = kaca.size();
	int head_x = kaca[size - 1][1];
	int head_y = kaca[size - 1][0];

	if ( head_x >= Dolzina or
		 head_x <= 0 or
		 head_y >= Visina or
		 head_y <= 0 ) return true;
	return false;
}

std::vector<int> novo_sadje( std::vector< std::vector<int>> & kaca )
{
	bool PASSED = false;
	int loc_x;
	int loc_y;

	while( !PASSED ){
		loc_x = std::rand() % (Dolzina - 1) + 1;
		loc_y = std::rand() % (Visina - 1) + 1;

		PASSED = true;
		int size = kaca.size();
		for ( int i = 0; i < size; i++ ){
			if( kaca[i][0] == loc_y and kaca[i][1] == loc_x ) PASSED = false;
		}
	}
	std::vector<int> location = {loc_y, loc_x};
	return location;
}

void narisi_sadje( std::vector< int > & sadje )
{
	move(sadje[0], sadje[1]);
	printw("s");
}

bool dotik_sadja( std::vector< int > & sadje , std::vector< std::vector< int>> & kaca)
{
	int size = kaca.size();
	if ( sadje[0] == kaca[size - 1][0] and sadje[1] == kaca[size - 1][1] ){
		return true;
	}
	return false;
}

void podaljsaj_kaco( Smer smer, std::vector< std::vector<int>> & kaca )
{
	int size = kaca.size();
	int head_x = kaca[size - 1][1];
	int head_y = kaca[size - 1][0];
	switch(smer)
	{
		case gor: kaca.push_back({head_y - 1, head_x}); break;
		case dol: kaca.push_back({head_y + 1, head_x}); break;
		case levo: kaca.push_back({head_y, head_x - 1}); break;
		case desno: kaca.push_back({head_y, head_x + 1}); break;
	}
	return;
}

