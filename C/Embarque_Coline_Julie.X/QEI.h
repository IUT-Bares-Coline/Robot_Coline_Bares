//
//// This is a guard condition so that contents of this file are not included
//// more than once.  
//#ifndef XC_HEADER_TEMPLATE_H
//#define	XC_HEADER_TEMPLATE_H
//
//#include <xc.h> // include processor files - each processor file is guarded.  
//
//
//#ifdef	__cplusplus
//extern "C" {
//#endif /* __cplusplus */
//
//
//#ifdef	__cplusplus
//}
//#endif /* __cplusplus */
//
//#endif	/* XC_HEADER_TEMPLATE_H */
#ifndef QEI_H
#define QEI_H

void InitQEI1();
void InitQEI2();

void QEIUpdateData();
void SendPositionData();

#endif
