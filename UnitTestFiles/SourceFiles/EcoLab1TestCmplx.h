//
// Created by Vladislav Volkov on 29.02.2024.
//

#ifndef ECO_LAB1_ECOLAB1TESTCMPLX_H
#define ECO_LAB1_ECOLAB1TESTCMPLX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <IEcoMemoryAllocator1.h>
#include "IEcoLab1.h"

#define EPS 1e-6

typedef struct ComplexNumber {
    double real, imag;
} Complex;

void CopyComplexNumber(Complex* number, Complex* copy) {
    copy->real = number->real;
    copy->imag = number->imag;
}

void *CreateRootsComplexArray(IEcoMemoryAllocator1* pIMem, size_t n_roots) {
    Complex *out = (Complex*) pIMem->pVTbl->Alloc(pIMem, n_roots * sizeof(Complex));
    srand(time(0));
    for (size_t i = 0; i < n_roots; ++i) {
        double range_real = 20.0;
        double range_imag = 10.0;
        out[i].real = ((double)rand() / (double)RAND_MAX) * range_real - 10.0;
        out[i].imag = ((double)rand() / (double)RAND_MAX) * range_imag - 5.0;
    }
    return out;
}

void DeleteComplexArray(IEcoMemoryAllocator1* pIMem, void* array_ptr) {
    pIMem->pVTbl->Free(pIMem, array_ptr);
}

void PrintComplexNumber(Complex* number) {
    if (number->imag >= EPS) {
        printf("%f+%fi ", number->real, number->imag);
    } else if (number->imag <= -EPS) {
        printf("%f%fi ", number->real, number->imag);
    } else {
        printf("%f ", number->real);
    }
}

void PrintComplexArray(void* array, size_t n_elems) {
    Complex* result = (Complex*) array;
    for (size_t i = 0; i < n_elems; ++i) {
        PrintComplexNumber(result + i);
    }
    printf("\n");
}

int InitComplexOnes(void* elem) {
    Complex cmplx;
    cmplx.imag = 0.0;
    cmplx.real = 1.0;
    CopyComplexNumber(&cmplx, (Complex*)elem);
    return 0;
}

int MultComplex(const void* first, const void* second, void* result) {
    Complex a = *((Complex*)first);
    Complex b = *((Complex*)second);
    Complex res;
    res.real = a.real * b.real - a.imag * b.imag;
    res.imag = a.imag * b.real + a.real * b.imag;
    CopyComplexNumber(&res, (Complex*)result);
    return 0;
}

int SubComplex(const void* first, const void* second, void* result) {
    Complex a = *((Complex*)first);
    Complex b = *((Complex*)second);
    Complex res;
    res.real = a.real - b.real;
    res.imag = a.imag - b.imag;
    CopyComplexNumber(&res, (Complex*)result);
    return 0;
}

void PrintComplexTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    printf("\n\nTestComplex\n");
    void* roots_ptr = CreateRootsComplexArray(pIMem, n_roots);
    printf("Roots:\n");
    PrintComplexArray(roots_ptr, n_roots);
    void* coefs;
    coefs = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(Complex));
    Complex* data = (Complex*)coefs;
    for (size_t i = 0; i <= n_roots; ++i) {
        data[i].imag = 0.0;
        data[i].real = 0.0;
    }
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(Complex), MultComplex, SubComplex, InitComplexOnes, coefs);
    printf("Coefficients:\n");
    PrintComplexArray(coefs, n_roots + 1);
    DeleteComplexArray(pIMem, roots_ptr);
}

void PrintComplexNumberToFile(Complex* number, FILE *file) {
    if (number->imag >= EPS) {
        fprintf(file, "%f+%fi", number->real, number->imag);
    } else if (number->imag <= -EPS) {
        fprintf(file, "%f%fi", number->real, number->imag);
    } else {
        fprintf(file, "%f", number->real);
    }
}

void WriteToFileComplexTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
    sprintf(csv_name, "ComplexResults_%d.csv", n_roots);
    errno_t code = fopen_s(&file, csv_name, "w");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    void* roots_ptr = CreateRootsComplexArray(pIMem, n_roots);
    void* coefs_ptr;
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(Complex));
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(Complex), MultComplex, SubComplex, InitComplexOnes, coefs_ptr);
    fprintf(file, "%s,%s\n", "Roots", "Coefficients");
    Complex* coefficients = (Complex*) coefs_ptr;
    Complex* roots = (Complex*) roots_ptr;
    for (size_t i = 0; i < n_roots; ++i) {
        PrintComplexNumberToFile(&roots[i], file);
        fprintf(file, ",");
        PrintComplexNumberToFile(&coefficients[i], file);
        fprintf(file, "\n");
    }
    fprintf(file, " ,");
    PrintComplexNumberToFile(&coefficients[n_roots], file);
    fprintf(file, "\n");
    fclose(file);
    DeleteComplexArray(pIMem, roots_ptr);
}

void WriteTimeToFileComplex(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
    sprintf(csv_name, "TimeComplex.csv");
    errno_t code = fopen_s(&file, csv_name, "a");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    void* roots_ptr = CreateRootsComplexArray(pIMem, n_roots);
    void* coefs_ptr;
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(Complex));
    clock_t begin, end;
    begin = clock();
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(Complex), MultComplex, SubComplex, InitComplexOnes, coefs_ptr);
    end = clock();
    double work_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
    fprintf(file, "%d,%f\n", n_roots, work_time);
    fclose(file);
    DeleteComplexArray(pIMem, roots_ptr);
}

#endif //ECO_LAB1_ECOLAB1TESTCMPLX_H
