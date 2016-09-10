/*
	Filename: Password.cpp
	Modified By: Logan Davis & Elliott Monday
	Date Modified: 9/09/2016
*/

#include "Password.h"
#include "ListArray.h"
#include "ListArrayIterator.h"
#include "Text.h"
using CSC2110::ListArray;
using CSC2110::ListArrayIterator;
using CSC2110::String;

#include <iostream>
using namespace std;

/* Constructor */
Password::Password()
{
   viable_words = new ListArray<String>();
   all_words = new ListArray<String>();
   len = -1;
}

/* Destructor */
Password::~Password()
{
   ListArrayIterator<String>* iter = all_words->iterator();
   
   // Delete every word the list iterator points to, then delete the iterator itself.
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

/* Method of adding new words to the password list. */
void Password::addWord(String* word)
{
	// Reset the referenced word length for the first word that is not an empty string.
	if (len == -1 && word->length() > 0)
	{
		len = word->length();
	}
	
	if (word->length() != len) { return; } // Do not add any strings that are not the same length.
	
	viable_words->add(word);
	all_words->add(word);
}

/* Method that compares the word passed to the list of viable passwords,
   and removes any passwords that don't have the same number of matching characters. */
void Password::guess(int try_password, int num_matches)
{
	String* word = all_words->get(try_password); // Retrieve the word given from the index.
	String* other = NULL;
	int numMatches;
	int index = 1;
	
	// First do a little error checking just to make sure everything is in range.
	if (try_password < 1 || try_password > all_words->size()) { return; }
	if (num_matches < 0 || num_matches > word->length()) { return; }
	
	// For every viable password that does not share the same amount of matching
	// characters found, remove it from the viable_words array list.
	while (index <= viable_words->size())
	{
		other = viable_words->get(index);
		numMatches = getNumMatches(word, other);
		if (numMatches != num_matches)
		{
			viable_words->remove(index);
		}
		else
		{
			index++;
		}
	}
}

/* Returns the number of viable passwords. */
int Password::getNumberOfPasswordsLeft()
{
	return viable_words->size();
}

/* Prints the list of viable passwords to the screen in its own body of text. */
void Password::displayViableWords()
{
	ListArrayIterator<String>* viableIter = viable_words->iterator();
	String* newLine = new String("\n");
	String* word = NULL;
	
	// Dedicate a new line on the screen for every viable password.
	newLine->displayString();
	while (viableIter->hasNext())
	{
		word = viableIter->next();
		word->displayString();
		newLine->displayString();
	}
	newLine->displayString();
	
	delete newLine;
	delete viableIter;
}

/* Determines the password that is most likely to be correct, and returns its index. (1-based) */
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

/* Returns a pointer to the string with the specified 1-based index. */
String* Password::getOriginalWord(int index)
{
	String* item = NULL;
	
	if (index >= 1 && index <= all_words->size())
	{
		item = all_words->get(index);
	}
	
	return item;
}

/* Calculates and returns the matching number of characters between the two words. */
int Password::getNumMatches(String* curr_word, String* word_guess)
{
	int numMatches = 0;
	int wordLength = curr_word->length(); // Words are assumed to be the same length.
	
	for (int i = 0; i < wordLength; i++)
	{
		if (curr_word->charAt(i) == word_guess->charAt(i))
		{
			numMatches++;
		}
	}
	
	return numMatches;
}
