#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>

enum EXIT_STATUS
{
    OK,
    INVALID_FLAG,
    INVALIG_FLAG_ARGUMENT,
    FILE_NOT_GIVEN,
    FILE_ERROR,
    INVALID_FLAG_COMBINATION,
    FLAG_NOT_GIVEN
};

void help(const char *name)
{
    puts("NAME");
    printf("\t%s - Calculate file content histogram\n\n", name);

    puts("SYNOPSIS");
    printf("\t %s [OPTION]... [FILE]\n\n", name);

    puts("DESCRIPTION");
    puts("\t Calculate histogram - frequency distribution - for all ASCII characters in a specified file.\n");
    puts("\t -r [SIZE]");
    puts("\t\t use 'read' function with specified buffer SIZE");
    puts("\t\t For more info about 'read' check 'man 2 read'\n");

    puts("\t -m");
    puts("\t\t use 'mmap' function");
    puts("\t\t For more info about 'mmap' check 'man 2 mmap'\n");

    puts("\t -i");
    puts("\t\t When finished, print histogram\n");

    puts("\t -t");
    puts("\t\t When done, print execution time\n");

    puts("\t -h");
    puts("\t\t Display help\n");

    puts("\tExit status:");
    puts("\t\t 0 \t if OK,");
    puts("\t\t 1 \t if an invalid flag was given,");
    puts("\t\t 2 \t if an invalid flag argument was given,");
    puts("\t\t 3 \t if the file was not specified,");
    puts("\t\t 4 \t if the file is not valid,");
    puts("\t\t 5 \t if an invalid flag combination was given,");
    puts("\t\t 6 \t if neither -r or -m was given.\n");
}

void calculate_hist(int hist[], const char buffer[], int n)
{
    for (size_t i = 0; i < n; i++)
    {
        hist[(int)buffer[i]]++;
    }
}

void print_hist(int hist[])
{
    puts("ASCII code : Number of occurrences");
    puts("-----------------------------------");
    for (size_t i = 0; i < 256; i++)
    {
        printf("%ld : %d\n", i, hist[i]);
    }
}

bool r_read(int fd, int buffer_size, int hist[])
{
    char buffer[buffer_size];
    int n;

    while ((n = read(fd, buffer, buffer_size - 1)) > 0)
    {
        if (n == -1)
        {
            perror("Error while reading from the file");
            return false;
        }
        buffer[n] = '\0';
        calculate_hist(hist, buffer, n);
    }
    return true;
}

bool m_mmap(int fd, int hist[])
{
    struct stat statbuf;
    if (fstat(fd, &statbuf) == -1)
    {
        perror("Error when getting file info");
        return false;
    }

    char *data = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("Error with mmap");
        return false;
    }

    for (size_t i = 0; i < statbuf.st_size; i++)
    {
        hist[(int)(*(unsigned char *)(&data[i]))]++;
    }

    if (munmap(data, statbuf.st_size) == -1)
    {
        perror("Error with munmap");
        return false;
    }

    return true;
}

void close_fd(int fd)
{
    if (close(fd) < 0)
    {
        perror("Error with file close");
        exit(FILE_ERROR);
    }
}

int main(int argc, char *argv[])
{
    int opt, buffer_size = 64;
    enum EXIT_STATUS exit_status = OK;
    bool r_flag = false, m_flag = false, i_flag = false, t_flag = false;

    if (argc == 1)
    {
        help(argv[0]);
        exit(exit_status);
    }

    while ((opt = getopt(argc, argv, "r:mith")) != -1)
    {
        switch (opt)
        {
        case 'r':
            r_flag = true;
            buffer_size = atoi(optarg);
            if (buffer_size <= 1)
            {
                fputs("Buffer size is invalid.\n", stderr);
                exit_status = INVALIG_FLAG_ARGUMENT;
                exit(exit_status);
            }
            break;
        case 'm':
            m_flag = true;
            break;
        case 'i':
            i_flag = true;
            break;
        case 't':
            t_flag = true;
            break;
        case 'h':
            help(argv[0]);
            exit(exit_status);
        default:
            help(argv[0]);
            exit_status = INVALID_FLAG;
            exit(exit_status);
        }
    }

    if (r_flag && m_flag)
    {
        fputs("Flags -r or -m can't be combined.\n", stderr);
        exit_status = INVALID_FLAG_COMBINATION;
        exit(exit_status);
    }

    if (!r_flag && !m_flag)
    {
        fputs("Provide either -r or -m flag.\n", stderr);
        exit_status = FLAG_NOT_GIVEN;
        exit(exit_status);
    }

    if (optind >= argc)
    {
        fputs("Expected file after options.\n", stderr);
        exit_status = FILE_NOT_GIVEN;
        exit(exit_status);
    }

    const char *file_name = argv[optind];
    int fd = open(file_name, O_RDONLY);

    if (fd == -1)
    {
        perror("Error when opening the file");
        exit_status = FILE_ERROR;
        exit(exit_status);
    }

    int hist[256];
    for (size_t i = 0; i < 256; i++)
    {
        hist[i] = 0;
    }

    bool ok = false;
    clock_t begin = clock();

    if (r_flag)
    {
        ok = r_read(fd, buffer_size, hist);
    }
    else if (m_flag)
    {
        ok = m_mmap(fd, hist);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    if (!ok)
    {
        close_fd(fd);
        exit_status = FILE_ERROR;
        exit(exit_status);
    }

    if (i_flag)
    {
        print_hist(hist);
    }

    if (t_flag)
    {
        printf("%f\n", time_spent);
    }

    close_fd(fd);
    return exit_status;
}