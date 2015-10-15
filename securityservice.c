/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "linkdb.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "hal_led.h"

#include "securityservice.h"
#include "st_util.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/* Service configuration values */
#define security_SERVICE_UUID    security_SERV_UUID

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Service UUID
static CONST uint8 securityServiceUUID[MW_UUID_SIZE] =
{
  MW_UUID(security_SERVICE_UUID), //0x9914
};

// Data UUID: data
static CONST uint8 securityDataUUID[MW_UUID_SIZE] =
{
  MW_UUID(security_DATA_UUID),
};

static CONST uint8 PublicKeyDataUUID[MW_UUID_SIZE] =
{
  MW_UUID(security_DataPublicKey_UUID ), //0x9915
};

static CONST uint8 rcvdSecretDataUUID[MW_UUID_SIZE] =
{
  MW_UUID(security_DatarcvdSecret_UUID), //0x9916
};

static CONST uint8 FlagDataUUID[MW_UUID_SIZE] =
{
  MW_UUID(security_DataFlag_UUID ), //0x9917
};

//Charactersitc UUID

CONST uint8 publicKeyUUID[MW_UUID_SIZE] =
{ 
  MW_UUID(security_PublicKey_UUID); //0x9918
};

CONST uint8 rcvdSecretUUID[MW_UUID_SIZE] =
{ 
  MW_UUID(security_rcvdSecret_UUID); //0x9919
};

CONST uint8 FlagUUID[MW_UUID_SIZE] =
{ 
  MW_UUID(security_Flag_UUID); //0x9920
};




/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * Profile Attributes - variables
 */

// Profile Service attribute
static CONST gattAttrType_t securityService = { MW_UUID_SIZE, securityServiceUUID };

// Characteristic Value: data
static uint8 securityData[security_DATA_LEN] = { 0 };
static uint8 publicKey[security_DATA_LEN] = {1234}; //sample
static uint8 rcvdSecret[security_DATA_LEN] = {0};
static uint8 FlagRx[security_DATA_LEN] = {0};
static uint8 secret = {4321}; //sample

// Characteristic Properties: data
static uint8 securityDataProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 publicKeyDataProps = GATT_PROP_READ;
static uint8 rcvdKeyDataProps = GATT_PERMIT_WRITE;
static uint8 FlagDataProps = GATT_PERMIT_READ;


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t securityAttrTable[] =
{
  {
    // Service declaration
    { ATT_BT_UUID_SIZE, primaryServiceUUID },   /* type */
      GATT_PERMIT_READ,                         /* permissions */
      0,                                        /* handle */
      (uint8 *)&securityService                  /* pValue */
    },
//----------------------------------------------
    // Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &securityDataProps
    },

    // Characteristic Value "Data"
    {
      { MW_UUID_SIZE, securityDataUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        securityData
    },
//-----------------------------------------------
	
	//Public Key Characteristic Declaration
	{
		{ATT_BT_UUID_SIZE,characterUUID},
		GATT_PERMIT_READ,
		0,
		&publicKeyDataProps
	},
	
	//Public Key Value (DATA)
	{
		{ATT_BT_UUID_SIZE,publicKeyDataUUID},
		GATT_PERMIT_READ,
		0,
		&publicKey
	},
//-------------------------------------------------	
	
	//-----------------------------------------------
	
	//Received Secret Characteristic Declaration
	{
		{ATT_BT_UUID_SIZE,characterUUID},
		GATT_PERMIT_READ,
		0,
		&rcvdKeyDataProps
	},
	
	//Received Secret Value (DATA)
	{
		{ATT_BT_UUID_SIZE,rcvdSecretDataUUID},
		GATT_PERMIT_WRITE,
		0,
		&rcvdSecret
	},
//-------------------------------------------------	

//-----------------------------------------------
	
	//Flag Characteristic Declaration
	{
		{ATT_BT_UUID_SIZE,characterUUID},
		GATT_PERMIT_READ,
		0,
		&FlagDataProps
	},
	
	//Public Key Value (DATA)
	{
		{ATT_BT_UUID_SIZE,FlagDataUUID},
		GATT_PERMIT_READ,
		0,
		&FlagRx
	},
//-------------------------------------------------	
	
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t security_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                   uint8_t *pValue, uint16_t *pLen,
                                   uint16_t offset, uint16_t maxLen,
                                   uint8_t method);
static bStatus_t security_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                    uint8_t *pValue, uint16_t len,
                                    uint16_t offset, uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// security Service Callbacks
CONST gattServiceCBs_t securityCBs =
{
  security_ReadAttrCB,    // Read callback function pointer
  security_WriteAttrCB,   // Write callback function pointer
  NULL                   // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      security_AddService
 *
 * @brief   Initializes the security service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   
 - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t security_AddService(uint32 services)
{
  uint8 status = SUCCESS;


  if ( services )
  {
       // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( securityAttrTable,
                                          GATT_NUM_ATTRS( securityAttrTable ),
										  GATT_MAX_ENCRYPT_KEY_SIZE,
                                          &securityCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      security_SetParameter
 *
 * @brief   Set a parameter. Allows *this application* to set a parameter. Parameters corresponds to the GATT characteristic values in the profile.
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
bStatus_t security_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;

  switch ( param )
  {
    case security_VALUE:
    if ( len == security_DATA_LEN )
    {
      VOID memcpy( securityData, value, security_DATA_LEN );
    }

    else
    {
      ret = bleInvalidRange;
    }
    break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return ( ret );
}

/*********************************************************************
 * @fn      security_GetParameter
 *
 * @brief   Get a parameter. Allows *the application* to set or change parameters in the profile; 
 *          these parameters correspond to the GATT characteristic values in the profile.
 *
 * @param   param - parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t security_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;

  switch ( param )
  {
    case security_VALUE:
      VOID memcpy( value, securityData, security_DATA_LEN );
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return ( ret );
}

/*********************************************************************
 * @fn          security_ReadAttrCB
 *
 * @brief       Read an attribute. Every time a GATT client device wants to read 
 *              from an attribute in the profile, this function gets called.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 security_ReadAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
        uint8_t *pValue, uint16_t *pLen,
        uint16_t offset, uint16_t maxLen,
        uint8_t method )
{
  uint16 uuid;
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }

  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }

  if (utilExtractUuid16(pAttr,&uuid) == FAILURE) {
    // Invalid handle
    *pLen = 0;
    return ATT_ERR_INVALID_HANDLE;
  }

  switch ( uuid )
  {
    // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
    // gattserverapp handles those reads
    case security_DATA_UUID:
      *pLen = security_DATA_LEN;
      // copy current security value in securityData[0] to the buffer pointed to by pValue
      memcpy( pValue, &securityData[0], security_DATA_LEN );
      securityData[0]++;
      break;
	  
	  case security_DataPublicKey_UUID:
      *pLen = security_DATA_LEN;
      // copy current security value in publicKey[0] to the buffer pointed to by pValue
      memcpy( pValue, &publicKey[0], security_DATA_LEN );
      publicKey[0]= 1234;
      break;
	  
	  case security_DataFlag_UUID:
      *pLen = security_DATA_LEN;
      // copy current security value in FlagRx[0] to the buffer pointed to by pValue
      memcpy( pValue, &FlagRx[0], security_DATA_LEN );
      FlagRx[0]=1;
      break;

    default:
      *pLen = 0;
      status = ATT_ERR_ATTR_NOT_FOUND;
      break;
    }

  return ( status );
}

/*********************************************************************
* @fn      security_WriteAttrCB
*
* @brief   Validate attribute data prior to a write operation. Every time a GATT 
*          client device wants to write to an attribute in the profile, 
*          this function gets called.
*
* @param   connHandle - connection message was received on
* @param   pAttr - pointer to attribute
* @param   pValue - pointer to data to be written
* @param   len - length of data
* @param   offset - offset of the first octet to be written
*
* @return  Success or Failure
*/
static bStatus_t security_WriteAttrCB( uint16_t connHandle, gattAttribute_t *pAttr,
        uint8_t *pValue, uint16_t len,
        uint16_t offset, uint8_t method )
{
  bStatus_t status = SUCCESS;
  uint16 uuid;

  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }

  if (utilExtractUuid16(pAttr,&uuid) == FAILURE) {
    // Invalid handle
    return ATT_ERR_INVALID_HANDLE;
  }

  switch ( uuid )
  {
    case security_DATA_UUID:
      // validate
      if ( offset != 0 ) 
      {
         status = ATT_ERR_INVALID_OFFSET;
      }
      if ( offset == 0 )
      {
        if ( len != 1 )
        {
          status = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      // Write the value
      if ( status == SUCCESS )
      {
        // Attribute consists of type|permissions|value|handle
        
        // pointer to the attribute's value part
        uint8 *pCurValue = (uint8 *)pAttr->pValue;
        
        // set attribute value to new value supplied in write operation
        *pCurValue = pValue[0];

        // save new value in variable        
        securityData[0] = pValue[0];
       // HalLedSet(HAL_LED_1, HAL_LED_MODE_ON );
      }
      break;
	  
	  case security_DatarcvdSecret_UUID:
      // validate
      if ( offset != 0 ) 
      {
         status = ATT_ERR_INVALID_OFFSET;
      }
      if ( offset == 0 )
      {
        if ( len != 1 )
        {
          status = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      // Write the value
      if ( status == SUCCESS )
      {
        // Attribute consists of type|permissions|value|handle
        
        // pointer to the attribute's value part
        uint8 *pCurValue = (uint8 *)pAttr->pValue;
        
        // set attribute value to new value supplied in write operation
        *pCurValue = pValue[0];

        // save new value in variable        
        rcvdSecret[0] = pValue[0];
       // HalLedSet(HAL_LED_1, HAL_LED_MODE_ON );
      }
      break;

    default:
      // Should never get here!
      status = ATT_ERR_ATTR_NOT_FOUND;
      break;
  }

  return ( status );
}


/*********************************************************************
*********************************************************************/