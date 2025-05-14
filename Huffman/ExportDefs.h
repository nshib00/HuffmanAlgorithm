#ifndef EXPORTDEFS_H
#define EXPORTDEFS_H

#include "pch.h"

#ifdef HUFFMAN_EXPORTS
#define HUFFMAN_API __declspec(dllexport)
#else
#define HUFFMAN_API __declspec(dllimport)
#endif

#endif // EXPORTDEFS_H
