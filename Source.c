#include <stdio.h>
#include <stdbool.h>


#define SIZE 9
#define ROW_BLOCK_SIZE 4
#define COL_BLOCK_SIZE 8
int map[2 * SIZE + 1][2 * SIZE + 1];

int players[2][2];
char players_pawn[2] = { 'x', 'o' };
int block_place[2];
int num_of_blocks[2] = { 20,20 };


char visual_map[4 * SIZE + 1][8 * SIZE + 1];

void init_table();
void print();
void add_block_at(int i, int j);
void add_pawn(int player, int i, int j);
void remove_pawn(int i, int j);
bool check_move(int player_pos[], int direction);
void apply_move(int player, int direction);
bool check_block(int place[], int direction);
void apply_block(int place[], int direction);
bool check_inside(int i, int j);

int main() {
	init_table();
	add_pawn(0,1,9);
	add_pawn(1,17,9);
	print();
	int player = 1;
	while (1) {
		int ans;
		printf("Player %c is turne:\n", players_pawn[player%2]);
		printf("1) Move\n2) Add Block(you have %d blocks)\n3) Exit\n", num_of_blocks[player%2]);
		printf("Choose one of the above: ");
		scanf("%d", &ans);
		if (ans == 1) {
			printf("Directions:\n1) Up\n2) Right\n3) Down\n4) Left\n");
			printf("Insert your direction: ");
			scanf("%d", &ans);
			if (check_move(players[player % 2], ans - 1)) {
				apply_move(player % 2, ans - 1);
				player++;
			}
			else {
				printf("Illegal move!\nTry agian...");
			}
		}
		else if (ans == 2) {
			printf("Directions:\n1) Horizontal\n2) Vertical\n");
			printf("Insert your direction: ");
			scanf("%d", &ans);
			printf("Insert place of the block: ");
			scanf("%d %d", &block_place[0], &block_place[1]);
			if (check_block(block_place, ans - 1) && num_of_blocks[player%2] > 0) {
				apply_block(block_place, ans - 1);
				num_of_blocks[player % 2]--;
				player++;
			}
			else {
				printf("Illegal move!\nTry agian...");
			}
		}
		else {
			return 0;
		}
		system("cls");
		print();
	}
}

void init_table() {
	for (int i = 0; i <= ROW_BLOCK_SIZE*SIZE; i++)
	{
		for (int j = 0;j <= COL_BLOCK_SIZE*SIZE;j++)
		{
			if (j%COL_BLOCK_SIZE == 0 && i%ROW_BLOCK_SIZE == 0)
				visual_map[i][j] = '+';

			else if (i%ROW_BLOCK_SIZE == 0)
			{
				if (j % 2 == 0)
					visual_map[i][j] = '*';
				else
					visual_map[i][j] = ' ';
			}
			else if (j%COL_BLOCK_SIZE == 0)
			{
				visual_map[i][j] = '*';
			}

			else
				visual_map[i][j] = ' ';
		}
	}
}

void add_pawn(int player, int i, int j)
{
	map[i][j] = 1;
	if (player == 1) {
		visual_map[i * ROW_BLOCK_SIZE / 2][j * COL_BLOCK_SIZE / 2] = 'o';
	}
	else {
		visual_map[i * ROW_BLOCK_SIZE / 2][j * COL_BLOCK_SIZE / 2] = 'x';
	}
	players[player][0] = i;
	players[player][1] = j;
}

void remove_pawn(int i, int j)
{
	map[i][j] = 0;
	visual_map[i * ROW_BLOCK_SIZE / 2][j * COL_BLOCK_SIZE / 2] = ' ';
}

bool check_move(int player_pos[], int direction) {
	int i = player_pos[0];
	int j = player_pos[1];

	switch (direction) {
	case 0: //Up
		return (map[i - 2][j] == 0 && i - 2 >= 0 && map[i - 1][j] == 0);
		break;
	case 1: //Right
		return (map[i][j + 2] == 0 && j + 2 <= 2 * SIZE && map[i][j + 1] == 0);
		break;
	case 2: //Down
		return (map[i + 2][j] == 0 && i + 2 <= 2 * SIZE && map[i + 1][j] == 0);
		break;
	case 3: //Left
		return (map[i][j - 2] == 0 && j - 2 >= 0 && map[i][j - 1] == 0);
		break;
	default:
		return false;
	}
}

void apply_move(int player, int direction) {
	int i = players[player][0];
	int j = players[player][1];
	remove_pawn(i, j);
	switch (direction) {
	case 0: //Up
		add_pawn(player, i - 2, j);
		break;
	case 1: //Right
		add_pawn(player, i, j + 2);
		break;
	case 2: //Down
		add_pawn(player, i + 2, j);
		break;
	case 3: //Left
		add_pawn(player, i, j - 2);
		break;
	}
}

bool check_inside(int i, int j)
{
	if (i < 0 || i > 2 * SIZE || j < 0 || j > 2 * SIZE)
		return false;
	return true;
}

bool check_block(int place[], int direction) {
	int i = place[0];
	int j = place[1];
	if (direction == 0) // horizontal
	{
		if (i == 1) // can not put block in first row
			return false;
		if (check_inside(i - 1, j) && check_inside(i - 1, j + 2) && !map[i - 1][j] && !map[i - 1][j + 2] && !map[i - 1][j + 1])
			return true;
		return false;
	}
	else if (direction == 1) // vertical
	{
		if (j == 1) // can not put block in first column
			return false;
		if (check_inside(i, j - 1) && check_inside(i + 2, j - 1) && !map[i][j - 1] && !map[i + 2][j - 1] && !map[i + 1][j - 1])
			return true;
		return false;
	}
	return false;
}

void add_block_at(int i, int j)
{
	map[i][j] = 1;
	if (i % 2 == 0 && j % 2 == 1)
	{
		i /= 2;
		j /= 2;
		i *= ROW_BLOCK_SIZE;
		j *= COL_BLOCK_SIZE;
		int jj;
		for (jj = j + 1;jj<j + 8;jj++)
			visual_map[i][jj] = '-';
	}
	else if (i % 2 == 1 && j % 2 == 0)
	{
		i /= 2;
		j /= 2;
		i *= ROW_BLOCK_SIZE;
		j *= COL_BLOCK_SIZE;
		int ii;
		for (ii = i + 1;ii<i + 4;ii++)
			visual_map[ii][j] = '|';
	}
}

void apply_block(int place[], int direction)
{
	int i = place[0];
	int j = place[1];
	add_block_at(i, j);
	if (direction) {
		add_block_at(i, j + 1);
		add_block_at(i, j + 2);
	}
	else {
		add_block_at(i + 1, j);
		add_block_at(i + 2, j);
	}
}

void print() {
	for (int i = 0; i <= ROW_BLOCK_SIZE*SIZE; i++)
	{
		for (int j = 0;j <= COL_BLOCK_SIZE*SIZE;j++)
		{
			printf("%c", visual_map[i][j]);
		}
		printf("\n");
	}
}
