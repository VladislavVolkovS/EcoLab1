/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IEcoLab1
 * </описание>
 *
 * <ссылка>
 *
 * </ссылка>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECOLAB1_H__
#define __I_ECOLAB1_H__

#include "IEcoBase1.h"

/* IEcoLab1 IID = {277FC00C-3562-4096-AFCF-C125B94EEC90} */
#ifndef __IID_IEcoLab1
static const UGUID IID_IEcoLab1 = {0x01, 0x10, 0x27, 0x7F, 0xC0, 0x0C, 0x35, 0x62, 0x40, 0x96, 0xAF, 0xCF, 0xC1, 0x25, 0xB9, 0x4E, 0xEC, 0x90};
#endif /* __IID_IEcoLab1 */

typedef struct IEcoLab1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void **ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ struct IEcoLab1* me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ struct IEcoLab1* me);

    /* IEcoLab1 */
	/*Функция poly, принимающая r - вектор корней полинома, возвращает коэффициенты полинома*/
	/*
		Parametrs:
			roots_data: void*
				Pointer to array of roots
			roots_count: size_t
				Number of roots
			type_size: size_t
				Size of one element
            mult: int (const void*, const void*, void*)
                Fuction of multiply
            sub: int (const void*, const void*, void*)
                Function of subtraction
            initOnes: int (void*)
                Initialization by ones
            coefs_data: void*
                Pointer to array of coefficients of the polynomial
		Returns:
			code: int16_t
				Return code
	*/
    int16_t (ECOCALLMETHOD *poly)(/* in */ struct IEcoLab1* me, /* in */ void* roots_data, size_t roots_count,
                                           size_t type_size,
                                           int (*mult)(const void* first, const void* second, void* res),
                                           int (*sub)(const void* first, const void* second, void* res),
                                           int (*initOnes)(void* res),
                                           /* out */ void* coefs_data);

} IEcoLab1VTbl, *IEcoLab1VTblPtr;

interface IEcoLab1 {
    struct IEcoLab1VTbl *pVTbl;
} IEcoLab1;


#endif /* __I_ECOLAB1_H__ */
