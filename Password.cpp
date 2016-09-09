#include "Password.h"
#include "ListArray.h"
#include "ListArrayIterator.h"
#include "Text.h"
using CSC2110::ListArray;
using CSC2110::ListArrayIterator;
using CSC2110::String;

#include <iostream>
using namespace std;

// Insert Methods here!
Password::Password()
{
   // NOTE: May need to change initialization later.
   viable_words = new ListArray();
   all_words = new ListArray();
   len = -1;
}

Password::~Password()
{
   ListArrayIterator<String>* iter = all_words->iterator();
   
   // Delete every word the list iterator points to, and delete the iterator itself.
   while (iter->hasNext())
   {
	   delete iter->next();
   }
   delete iter;
   iter = NULL;
   
   delete viable_words;
   delete all_words;
   viable_words = NULL;
   all_words = NULL;
}

void Password::addWord(String* word)
{
	if (word->length() == 0) { return; } // Additional safeguard for empty strings. May need to remove.
	
	// Reset the referenced word length for the first word that is added.
	if (len == -1)
	{
		len = word->length();
	}
	viable_words->add(word);
	all_words->add(word);
}

void Password::guess(int try_password, int num_matches)
{
	// Insert code
}

int Password::getNumberOfPasswordsLeft()
{
	return viable_words->size();
}

void Password::displayViableWords()
{
	ListArrayIterator<String>* viableIter = viable_words->iterator();
	String* word = NULL;
	
	while (viableIter->hasNext)
	{
		word = viableIter->next();
		word->displayText(); // NOTE: May encounter '\n' problems during runtime.
	}
	
	delete viableIter;
}

int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}

String* Password::getOriginalWord(int index) // 1-based
{
	String* item = NULL;
	
	if (index >= 1 && index <= all_words->size())
	{
		item = all_words->get(index);
	}
	
	return item;
}
