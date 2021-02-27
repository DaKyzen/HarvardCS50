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

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

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
    bool isValid = false;
    for (size_t i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i; // ranks[rank] will be set to the ith position in candidate. 
            isValid = true;  // E.g. When i = 1 and name = "Alice", ranks[rank] will hold 1 which can be used to get "Alice" in candidates
        }
    }
    
    if (isValid)
        return true;
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Looping over ranks with starting candidate at rank 1
    for (size_t i = 0; i < candidate_count - 1; i++) 
    {
        // Second for loop starts comparing starting candidtate to all those who has a lower rank, ignoring candidates with a higher rank
        for (size_t j = i + 1; j < candidate_count; j++)
        {
            int current = ranks[i];
            int lowerCandidate = ranks[j];

            preferences[current][lowerCandidate] ++;
            
            // // Testing
            // for (size_t k = 0; k < candidate_count; k++)
            // {
            //     printf("%s, ", candidates[k]);
            // }
            // printf("\nCurrent: %s \n", candidates[ranks[i]]);
            // printf("Lower Candidate: %s \n", candidates[ranks[j]]);
            // printf("Preference for Current: %i \n\n", preferences[current][lowerCandidate]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int position = 0; // Position in pair array
    pair_count = 0;
    for (size_t i = 0; i < candidate_count - 1; i++) 
    {
        for (size_t j = i + 1; j < candidate_count; j++)
        {
            bool isATie = (preferences[i][j] == preferences[j][i]);
            if (!isATie)
            {
               if (preferences[i][j] > preferences[j][i])
               {
                   pairs[position].winner = i;
                   pairs[position].loser = j;
               }
               else if (preferences[i][j] < preferences[j][i])
               {
                   pairs[position].winner = j;
                   pairs[position].loser = i;
               }
                position ++;
                pair_count ++;
            }
        }
    }
    // // Testing
    // for (size_t i = 0; i < pair_count; i++)
    // {
    //     printf("Winner: %s\n", candidates[pairs[i].winner]);
    //     printf("Loser: %s\n", candidates[pairs[i].loser]);
    //     printf("Points: %i\n", preferences[pairs[i].winner][pairs[i].loser]);
    // }
    return;
}

// Sort pairs in decreasing order by strength of victory using selection sort
void sort_pairs(void)
{
    int startingPairPos = 0;
    int largestPairPos = 0;
    if (pair_count > 0)
    {
        for (size_t i = 0; i < pair_count - 1; i++)
        {
            startingPairPos = i;
            largestPairPos = i;

            int startingPairPreference = preferences[pairs[i].winner][pairs[i].loser];

            for (size_t j = i + 1; j < pair_count; j++)
            {
                int currentPairPreference = preferences[pairs[j].winner][pairs[j].loser];

                if (currentPairPreference > startingPairPreference)
                    largestPairPos = j;
            }
            
            pair temp = pairs[i];
            pairs[i] = pairs[largestPairPos];
            pairs[largestPairPos] = temp;
        }

    }
    // // Testing
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("\n%i---\n", i);
    //     printf("Winner: %s\n", candidates[pairs[i].winner]);
    //     printf("Loser: %s\n", candidates[pairs[i].loser]);
    //     printf("Points: %i\n", preferences[pairs[i].winner][pairs[i].loser]);
    // }    
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // There are no cycles as long as one candidate has nothing pointed to it. Other candidates may have as many pointers to it as possible
    bool candidatesWithPointersToIt[MAX] = { 0 }; // true if candidate has a pointer to it

    for (size_t i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        bool isValid = false; // true if there still exists a candidate without a pointer to it

        candidatesWithPointersToIt[loser] = true;
        
        // Check if there is a candidate without a pointer to it
        for (size_t j = 0; j < candidate_count; j++)
        {
            if (candidatesWithPointersToIt[j] == false) // If there is at least one candidate with no pointer to it the isValid = true
                isValid = true;
        }
        
        if (isValid)
            locked[winner][loser] = true;
        else
            candidatesWithPointersToIt[loser] = false;
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool candidatesWithPointersToIt[MAX] = {false}; // true if pointers to it
    int winner = -1;
    for (size_t i = 0; i < candidate_count; i++)
    {
        for (size_t j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
                candidatesWithPointersToIt[j] = true;
        }
    }
    for (size_t i = 0; i < candidate_count; i++)
    {
        if (candidatesWithPointersToIt[i] == false)
            winner = i;
    }
    
    printf("%s\n", candidates[winner]);
    
    return;
}
