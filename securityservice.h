#ifndef securitySERVICE_H
#define securitySERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 Data constants for the app : 
 Data1 - public key (READ)
 data2 - secret (CONSTANT) //not a characterestic. Just something defined in the c file.
 Data3 - rcvd_secret (WRITE)
 Data4 - Flag (READ)
 */

/*********************************************************************
 * CONSTANTS
 */

// MW Base 128-bit UUID: 3E09XXXX-293F-11E4-93BD-AFD0FE6D1DFD
#define MW_UUID_SIZE        ATT_UUID_SIZE
#define MW_UUID(uuid)       MW_BASE_UUID_128(uuid)
 
// security Service Parameters
#define security_VALUE                     0

// Service UUID
#define security_SERV_UUID               0x9914  // 3E099914-293F-11E4-93BD-AFD0FE6D1DFD
#define security_DataPublicKey_UUID              0x9915 //holds the public key
#define security_DatarcvdSecret_UUID              0x9916 // holds the rcvd_secret
#define security_DataFlag_UUID              0x9917 //holds the flag value . TRUE when secret = rcvd_secret.

// Characteristic UUID
#define security_PublicKey_UUID 	     0x9918
#define security_rcvdSecret_UUID         0x9919
#define security_Flag_UUID               0x9920 
  
// security length
#define security_DATA_LEN                       20

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

 // MW Base 128-bit UUID: 3E09XXXX-293F-11E4-93BD-AFD0FE6D1DFD
#define MW_BASE_UUID_128( uuid )  0xFD, 0x1D, 0x6D, 0xFE, 0xD0, 0xAF, 0xBD, 0x93, \
                                  0xE4, 0x11, 0x3F, 0x29, LO_UINT16( uuid ), HI_UINT16( uuid ), 0x09, 0x3E
 
/*********************************************************************
 * Profile Callbacks
 */


/*********************************************************************
 * API FUNCTIONS
 */

/*
 * security_AddService- Initializes the security service by registering
 *          GATT attributes with the GATT server.
 *
 */

extern bStatus_t security_AddService(uint32 services );

/*********************************************************************
 * @fn      security_SetParameter
 *
 * @brief   Set a security parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t security_SetParameter( uint8 param, uint8 len, void *value );

/*
 * security_GetParameter - Get a security parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t security_GetParameter( uint8 param, void *value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* securitySERVICE_H */
