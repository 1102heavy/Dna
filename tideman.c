#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int rejected_winner;
int rejected_loser;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool recursion(int lock[], int m);
bool is_cycle(int m);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            for (int k = 0; k < candidate_count; k++)
            {
                for (int z = 0; z < candidate_count; z++)
                {
                    if (ranks[k] == i && ranks[z] == j && k < z)
                    {
                        preferences[i][j]++;
                        //printf("%i", preferences[i][j]);
                    }
                }
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
// using linear sort with the help of for loop
void sort_pairs(void)
{
    // TODO
    int Arrays[pair_count - 1];
    for (int i = 0; i < pair_count - 1; i++)
    {
        Arrays[i] = preferences[pairs[i].winner][pairs[i].loser];
    }
    for (int i = 0; i < pair_count - 1; i++)
    {
        int biggest = Arrays[i];
        int biggestnumberplace = i;
        for (int j = i; j < pair_count; j++)
        {
            if (Arrays[j] > Arrays[i])
            {
                biggest = Arrays[j];
                biggestnumberplace = j;
            }
        }
        if (biggest != Arrays[i])
        {
            //swaping using tmp var
            int number = Arrays[i];
            Arrays[i] = Arrays[biggestnumberplace];
            Arrays[biggestnumberplace] = number;
        }
    }

    for (int z = 0; z < pair_count; z++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            if (Arrays[z] == preferences[pairs[j].winner][pairs[j].loser])
            {
                // swaping using tmp var
                pair p = pairs[z];
                pairs[z] = pairs[j];
                pairs[j] = p;
            }
        }
    }


    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        printf("locked[%i][%i]", pairs[i].winner, pairs[i].loser);
        locked[pairs[i].winner][pairs[i].loser] = true;
        // Trying to figure out recursion
        if (is_cycle(i))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
            rejected_winner = pairs[i].winner;
            rejected_loser = pairs[i].loser;
        }

    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int arrows = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j)
            {
                continue;
            }
            else if (locked[j][i] == false)
            {
                arrows++;
            }
        }
        if (arrows == candidate_count - 1)
        {
            printf("%s\n", candidates[i]);
        }
    }
// working with true
    /*
    only able to print when not tied
    but I don't know why?

    int winner[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        winner[i] = 0;
    }
    for ( int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if ( j == i)
            {
                continue;
            }
            else if (locked[i][j] == true)
            {
                winner[i] = winner[i] + 99;
            }
            else if (locked[i][j] == false && i == rejected_winner && j == rejected_loser)
            {
                continue;
            }
                    else if (locked[i][j] == false)
                    {
                        winner[i] = winner[i] - 99999;
                    }
               }
           }

        for (int i = 0; i < candidate_count; i++)
        {
             if (winner[i] > 0)
            {
                printf("%s\n", candidates[i]);
            }
        }
    */

}
// setting up the base case of recursion
// using first function
bool is_cycle(int m)
{
    int lock[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        // setting every tmp lock to false
        lock[i] = false;
    }
    if (recursion(lock, pairs[m].winner))
    {
        return true;
    }
    return false;
}

// Doing actual recursion by calling
// function inside a function to
// to creat/detect cycle
bool recursion(int lock[], int m)
{
    // Setting up tmp locks
    if (lock[m] == true)
    {
        return true;
    }
    else
    {
        lock[m] = true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[m][i] && recursion(lock, i))
        {
            return true;
        }
    }
    return false;
}