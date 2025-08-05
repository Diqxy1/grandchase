#pragma once
//========================================================================
// KPrimeSearch.h -  traverse a known set of items randomly only once
//
// Part of the GameCode Application
//
// GameCode is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete" by Mike McShaffry, published by
// Paraglyph Press. ISBN: 1-932111-75-1
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: http://www.amazon.com/exec/obidos/ASIN/1932111751/gamecodecompl-20/
//
// There's also a companion web site at http://www.mcshaffry.com/GameCode/portal.php
//
// (c) Copyright 2003 Michael L. McShaffry
//
// This work is licensed under the Creative Commons Attribution-ShareAlike License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/1.0/
// or send a letter to:
//      Creative Commons
//      559 Nathan Abbott Way
//      Stanford, California 94305, USA.
//
//========================================================================

//========================================================================
//
//  Original Code written at Compulsive Development
//
//========================================================================

//========================================================================
//  Content References in Game Coding Complete
//
// class KPrimeSearch				- Chapter 3, pages 93-97
//
//========================================================================

/******************************************************************
This class enables you to visit each and every member of an array
exactly once in an apparently random order.

An application of this algorithim would be a good pixel fade in/fade out
where each pixel of the frame buffer was set to black one at a time.

Here's how you would implement a pixel fade using this class:

void FadeToBlack(Screen *screen)
{
	int w = screen.GetWidth();
	int h = screen.GetHeight();

	int pixels = w * h;

	KPrimeSearch search(pixels);

	int p;

	while((p=search.GetNext())!=-1)
	{
		int x = p % w;
		int y = h / p;

		screen.SetPixel(x, y, BLACK);

		// of course, you wouldn't blit every pixel change.
		screen.Blit();
	}
}

NOTE: If you want the search to start over at the beginning again -
you must call the Restart() method, OR call GetNext(true).

********************************************************************/

class KPrimeSearch
{
	static int prime_array[];

	int skip;
	int currentPosition;
	int maxElements;
	int *currentPrime;
	int searches;

public:
	KPrimeSearch(int elements, unsigned int uiRandomSeed);
	int GetNext(bool restart = false);
	bool Done() { return (searches == *currentPrime); }
	void Restart() { currentPosition = 0; searches = 0; }
};
