/*********************************************************************
File name: main.cpp
Author: Luke Lyons (ujw18)
Date: 2025-09-16

Purpose:
  Load, display, and update a movie collection from a file.

Command Parameters:
  None (interactive).

Input:
  - Filename at prompt.
  - File: first line N, then N lines:
    Title_with_underscores Director_with_underscores runtime watched year
  - User enters a movie number (0..N-1) or "quit".

Results:
  - Prints a table; increments Watched for the chosen movie.
  - On quit: prints total runtime and least-watched title.

Notes:
  - Underscores → spaces on load; capacity 200; indexes are 0-based.
*********************************************************************/

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<algorithm>

struct Movie 
{
    std::string szTitle;
    std::string szDirector;
    int iRuntime;
    int iWatched;
    int iYear;
};

bool loadMovies(std::string& szFilename, Movie moviesArr[], int& iSize);
void displayMovies(const Movie moviesArr[], int iSize);
void watchMovie(Movie moviesArr[], int iSize, int iMovieIndex);
int calculateTotalRuntime(const Movie moviesArr[], int iSize);
int findLeastWatched(const Movie moviesArr[], int iSize);

/*********************************************************************
function prototype
Purpose:
    Program entry point. Loads movies, displays the table, processes
    user selections, and on "quit" prints summary stats.

Parameters:
    (none)

Return Value:
    int – 0 on normal exit; -1 if the input file could not be opened.

Notes:
    Loop accepts either a zero-based movie index or the word "quit".
*********************************************************************/

int main()
{
    std::string szFilename;
    int iNumMovies;
    Movie moviesArr[200];

    std::cout << "Please enter a file to open: ";
    std::cin >> szFilename;

    bool bIsLoaded = loadMovies(szFilename, moviesArr, iNumMovies);
    if(!bIsLoaded)
    {
        std::cout << szFilename << " not found. Exiting Program";
        return -1;
    };
    
    while(true)
    {
        displayMovies(moviesArr, iNumMovies);
        
        std::string szSelectedMovie;
        std::cout << "Enter the movie number to watch (or \"quit\"): ";
        std::cin >> szSelectedMovie;

        if(szSelectedMovie == "quit")
        {
            int iTotalRuntime = calculateTotalRuntime(moviesArr, iNumMovies);
            int iLeastWatched = findLeastWatched(moviesArr, iNumMovies);

            std::cout << std::endl;
            std::cout << "Total runtime of all movies: " << iTotalRuntime << std::endl;
            std::cout << "Least watched movie: " << moviesArr[iLeastWatched].szTitle << std::endl;
            break;
        };
        int iSelectedMovie = std::stoi(szSelectedMovie);
        watchMovie(moviesArr, iNumMovies, iSelectedMovie); 
    };

    return 0;
}

/*********************************************************************
function prototype
Purpose:
    Load movies from a text file into the array and report how many
    were loaded.

Parameters:
    I   std::string& szFilename  - Name of the file to open 
    (not modified)
    O   Movie moviesArr[]        - Output array; elements 
    [0..iSize-1] filled
    O   int& iSize               - Set to number of movies loaded

Return Value:
    bool – false if file could not be opened; true otherwise.

Notes:
    Expects first integer N, then N lines:
      Title_with_underscores Director_with_underscores runtime 
      watched 
      year
    Converts underscores to spaces for title/director.
*********************************************************************/

bool loadMovies(std::string& szFilename, Movie moviesArr[], int& iSize)
{
    std::ifstream inputFile;
    inputFile.open(szFilename);

    if(!inputFile.is_open())
    {
        return false;
    };
    
    int iNumMovies;
    inputFile >> iNumMovies;

    std::string szRawTitle, szRawDirector;
    int iRuntime, iCopies, iYear;

    for(int i = 0; i < iNumMovies; i++)
    {
        inputFile >> szRawTitle >> szRawDirector >> iRuntime >> iCopies >> iYear;

        std::replace(szRawTitle.begin(), szRawTitle.end(), '_', ' ');
        std::replace(szRawDirector.begin(), szRawDirector.end(), '_', ' ');

        moviesArr[i].szTitle = szRawTitle; 
        moviesArr[i].szDirector = szRawDirector;
        moviesArr[i].iRuntime = iRuntime;
        moviesArr[i].iWatched = iCopies;
        moviesArr[i].iYear = iYear;
    };

    iSize = iNumMovies;
    inputFile.close();
    return true;
};

/*********************************************************************
function prototype
Purpose:
    Print a formatted table of movies to standard output.

Parameters:
    I   const Movie moviesArr[]  - Array of movies to display
    I   int iSize                - Number of valid movies

Return Value:
    void

Notes:
    Left-aligned columns; indexes shown are zero-based.
*********************************************************************/

void displayMovies(const Movie moviesArr[], int iSize)
{
    std::cout << std::left
              << std::setw(3) << "N"
              << std::setw(25) << "Title"
              << std::setw(20) << "Director"
              << std::setw(8) << "Runtime"
              << std::setw(10) << "Watched"
              << std::setw(6) << "Year"
              << std::endl;

    for (int i = 0; i < iSize; i++) {
        const Movie& m = moviesArr[i];

        std::cout << std::setw(3) << i 
                  << std::setw(25) << m.szTitle
                  << std::setw(20) << m.szDirector
                  << std::setw(8) << m.iRuntime
                  << std::setw(10) << m.iWatched
                  << std::setw(6) << m.iYear
                  << std::endl;
    }
    std::cout << std::endl;
};

/*********************************************************************
function prototype
Purpose:
    Increment the watched count for the selected movie.

Parameters:
    I/O Movie moviesArr[]  - Array whose selected element is modified
    I   int iSize          - Number of valid movies
    I   int iMovieIndex    - Zero-based index of movie to mark watched

Return Value:
    void

Notes:
    If iMovieIndex >= iSize, prints "No movie to watch" and returns
    without modifying data. (Assumes iMovieIndex >= 0.)
*********************************************************************/

void watchMovie(Movie moviesArr[], int iSize, int iMovieIndex)
{
    if(iMovieIndex >= iSize)
    {
        std::cout << "No movie to watch" << std::endl;
        return;
    };

    moviesArr[iMovieIndex].iWatched += 1;
};

/*********************************************************************
function prototype
Purpose:
    Compute the sum of all runtimes.

Parameters:
    I   const Movie moviesArr[]  - Array of movies
    I   int iSize                - Number of valid movies

Return Value:
    int – total runtime in minutes.

Notes:
    Sums moviesArr[i].iRuntime for i in [0, iSize).
*********************************************************************/

int calculateTotalRuntime(const Movie moviesArr[], int iSize)
{
    int iTotalRuntime = 0;

    for(int i = 0; i < iSize; i++)
    {
        iTotalRuntime += moviesArr[i].iRuntime;
    };
    
    return iTotalRuntime;
};

/*********************************************************************
function prototype
Purpose:
    Find the index of the least-watched movie.

Parameters:
    I   const Movie moviesArr[]  - Array of movies
    I   int iSize                - Number of valid movies

Return Value:
    int – zero-based index of the movie with the smallest iWatched.

Notes:
    Assumes iSize > 0. On ties, returns the first (lowest index).
*********************************************************************/

int findLeastWatched(const Movie moviesArr[], int iSize)
{
    int iLeastWatchedTime = moviesArr[0].iWatched;
    int iLeastWatchedIndex = 0;

    for(int i = 1; i < iSize; i++)
    {
        if(moviesArr[i].iWatched < iLeastWatchedTime)
        {
            iLeastWatchedTime = moviesArr[i].iWatched;
            iLeastWatchedIndex = i;
        };
    };

    return iLeastWatchedIndex;
};
