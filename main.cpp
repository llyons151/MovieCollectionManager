/*********************************************************************
File name: main.cpp
Author: <Luke Lyons>(<ujw18>)
Date: 16/09/2025

Purpose:
   Description of the purpose of the program.
Command Parameters:
   -
Input:
   Description of the input to the program.
Results:
   Description of what the program produces.
Notes:
   Description of any special information regarding this program.
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

int main()
{
    std::string szFilename;
    int iNumMovies;
    int iSize;
    Movie moviesArr[200];

    std::cout << "Please enter a file to open: ";
    std::cin >> szFilename;

    bool bIsLoaded = loadMovies(szFilename, moviesArr, iSize);
    if(!bIsLoaded)
    {
        std::cout << "not found. Exiting program";
        return -1;
    };
    
    while(true)
    {
        displayMovies(moviesArr, iSize);
        
        std::string szSelectedMovie;
        std::cout << "Enter the movie number to watch (or \"quit\"): ";
        std::cin >> szSelectedMovie;

        if(szSelectedMovie == "quit")
        {
            int iTotalRuntime = calculateTotalRuntime(moviesArr, iSize);
            int iLeastWatched = findLeastWatched(moviesArr, iSize);

            std::cout << "Total runtime of all movies: " << iTotalRuntime << std::endl;
            std::cout << "Least watched movie: " << iLeastWatched << std::endl;
            break;
        };
        int iSelectedMovie = std::stoi(szSelectedMovie);
        watchMovie(moviesArr, iSize, iSelectedMovie); 
    };

    return 0;
}

bool loadMovies(std::string& szFilename, Movie moviesArr[], int& iSize)
{
    std::ifstream inputFile;
    inputFile.open(szFilename);

    if(!inputFile.is_open())
    {
        std::cout << "Error: Could not open file for reading.";
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

        std::cout << std::setw(3) << i + 1
                  << std::setw(25) << m.szTitle
                  << std::setw(20) << m.szDirector
                  << std::setw(8) << m.iRuntime
                  << std::setw(10) << m.iWatched
                  << std::setw(6) << m.iYear
                  << std::endl;
    }
};

void watchMovie(Movie moviesArr[], int iSize, int iMovieIndex)
{
    if(iMovieIndex >= iSize)
    {
        std::cout << "No movie to watch" << std::endl;
        return;
    };

    moviesArr[iMovieIndex].iWatched += 1;
};

int calculateTotalRuntime(const Movie moviesArr[], int iSize)
{
    int iTotalRuntime = 0;

    for(int i = 0; i < iSize; i++)
    {
        iTotalRuntime += moviesArr[i].iRuntime;
    };
    
    return iTotalRuntime;
};

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




