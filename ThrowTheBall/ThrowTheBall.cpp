#include <iostream>
#include <vector>

//g++ ThrowTheBall.cpp -o ThrowTheBall
//./ThrowTheBall


/*
Problem (2) ThrowTheBall:

Initial thoughts:

This problem is definitely at most O(n) because you have to check each element in the N friends setting in a circle, 
so if I can find an O(n) brute force solution, 
I’m set. It is pretty easy to tell when the game starts (could be anywhere) and the players take turns passing the ball to each other until one of player has alreadly received the ball M times, then the game is over. 
If I iterate through the integer array and check if the number of receiving ball condition is larger than M time.
then I can increment the passing count value.

Pseudo-Code:

class ThrowTheBall
	public procedure ThrowTheBall (int N, int M, int L)
		noplayers=N;
		helds=M;
		num_passes=L;
	endprocedure
	public procedure timesThrown()
		next = 0
		passing_count = 0
		playerlist
		playerlist[next]++;
		Repeat i<- playerlist[next]+1 until playerlist[next]<helds
			playerlist[next]+1
			switch(playerlist[next] % 2)
				case 0:
				if (next + num_passes) < noplayers
					next=(next + num_passes);
				else
					next=(next + num_passes - noplayers);
				default:
				if (next + noplayers - num_passes) < noplayers
					next=(next + noplayers - num_passes);
				else
					next=(next - num_passes);
		EndRepeat
		return passing_count;
	}
};


Reflection:

Because the players are the ones who pass the football together, 

every time the whole system has to make a record, 

every time the world checks whether the number of passing has the larger than the maximum passing number, 

so I need to check whether the number is even number, 

and check the total number of passing footballs smaller than the maximum passing number. 

In order to check if the pass has been passed to the next lap, 

then I have to check the sum of the index of the person and passing ball number is less than the total number of people.

Let setup a example, the number of players is 5, and they are sitting in a circle, clockwise numbered from 1 to 5. 

When starting the game, Player 1 will catches the ball and pass it to another players. 

And the maximum time of helding the ball is 3 and only 2 passing_count the ball.
 
So if any player catches the ball 3 times, then the game ends.

Finally, the output is 10


The first of all, the first player 1 gets the ball. Because he had a ball, he passed the ball to the player 4. 

Who is the two places on his right. Now, this is the first time the player 4 catches the ball, so he gave it

Player 2, pass the ball to the player 5. Player 5 and pass the ball to the player 3. Who passing_count the ball

Back to player 1. Because player 1 has already held the ball twice, so pass the ball to player 3, who

Pass the ball to player 5, then the player passing_count the ball to player 2. Finally, player 2 passing_count the ball to the player

Then player 4 passing_count the ball to the player 1. Player 1 now passing_count the ball to the player 3 times, then the game end.

*/


class ThrowTheBall
{
private:
	int noplayers;
	int num_passes;
	int helds;
public:
	int timesThrown(int N, int M, int L)
	{
		int noplayers=N;
		int helds=M;
		int num_passes=L;

		int next = 0;
		int passing_count = 0;
		std::vector<int> playerlist(noplayers);
		playerlist[next]++;
		for(int i=playerlist[next];playerlist[next]<helds;++playerlist[next]){
			++passing_count;
			switch(playerlist[next] % 2)
			{
				case 0:
				if((next + num_passes) < noplayers){
					next=(next + num_passes);
				}
				else{
					next=(next + num_passes - noplayers);
				}
				break;
				default:
				if((next + noplayers - num_passes) < noplayers){
					next=(next + noplayers - num_passes);
				}else{
					next=(next - num_passes);
				}
				break;
			}
		}
		return passing_count;
	}
};

int main()
{
	ThrowTheBall bp;
	std::cout << "\nTotal passing_count:" << bp.timesThrown(5,3,2)<<std::endl;
	return 0;
}