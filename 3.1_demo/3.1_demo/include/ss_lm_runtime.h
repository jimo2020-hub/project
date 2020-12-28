/** 
*   @file       ss_lm_runtime.h
*   @brief      Core header for LicenseManagent. ���ɹ����õĵ���ͷ�ļ�. 
*               ��Ӧ��̬��: slm_runtime.dll/slm_runtime_dev.dll
*               ��Ӧ��̬��: slm_runtime_api.lib/slm_runtime_api_dev.lib
*   @details    license runtime api head files. ����API�û�����ʱ�ӿ�ͷ�ļ�.
*   
*   history:
*   
*   �������Java��Dotnet��DLL��װ���� ��ο� slm_runtime_easy.h ͷ�ļ�
*   slm_keep_alive �ǵ�¼�����Ҫ���ã��� Virbox���ɷ��� ֪��APPӦ�û������ţ�û���������߱����ԡ�
*   
*   ʹ�����ɵ���˳��
*       ��һ��������slm_init��������ȫ�ֳ�ʼ��
*       �ڶ���������slm_login������¼���ɣ�Ӳ�����ɣ��������ɣ������ɣ������ɣ�
*       ���������������ɲ�������������slm_keepalive slm_encrypt slm_decrypt�Ȳ���
*       ���Ĳ�������slm_logout�������ǳ���ǰ����
*       ���岽�������˳�ʱ������slm_cleanup��������ʼ������ѡ���ã�
* 
*   @version   2.1.0
*   @date      2013-20117
*   @pre       ������֤��(��˼ǩ���������� Virbox���ɷ��� ����֤)��Vdata(APP����Ϣ���ÿ�����ǩ��)��ECC��Կ(�˵�����֤�����)��Ҫ����lib
*
*/
#ifndef __SS_LM_RUMTIME_H__
#define __SS_LM_RUMTIME_H__
#include "ss_define.h"
//#include "ss_error.h"

#ifdef  _MSC_VER
#pragma comment(linker, "/defaultlib:ws2_32.lib")
#pragma comment(linker, "/defaultlib:iphlpapi.lib")
#pragma comment(linker, "/defaultlib:psapi.lib")
#pragma comment(linker, "/defaultlib:Shlwapi.lib")
#pragma comment(lib, "rpcrt4.lib")


#if _MSC_VER >= 1900   // 1900��VS2015�İ汾�ţ������̬����VS2015�±���ʧ�ܵ�����
#pragma comment(linker, "/defaultlib:legacy_stdio_definitions.lib")
#endif  // MSC_VER

#endif//???
/** �������Ϣ����ö��  */
typedef enum _INFO_TYPE {
    /**  Ӳ��������Ϣ  */
    LOCK_INFO = 1,
    /**  �������ɵĻỰ��Ϣ */
    SESSION_INFO = 2,
    /**  ��ǰ��¼��������Ϣ */
    LICENSE_INFO  = 3,
	/**  �����ļ��б� */
	FILE_LIST  = 4,
    
} INFO_TYPE;

/** ��������������ö��  */ 
typedef enum _LIC_USER_DATA_TYPE {
    /** ֻ���� */
    ROM = 0,     
    /** ��д�� */
    RAW = 1,
    /** ������ */
    PUB = 2,
} LIC_USER_DATA_TYPE;


/** Ӳ����������ɫ����ɫ*/
#define LED_COLOR_BLUE      0
/** Ӳ����������ɫ����ɫ*/
#define LED_COLOR_RED       1

/** Ӳ�������ƿ��ƣ��ر�*/
#define LED_STATE_CLOSE     0
/** Ӳ�������ƿ��ƣ���*/
#define LED_STATE_OPEN      1
/** Ӳ�������ƿ��ƣ���˸*/
#define LED_STATE_SHRINK    2

/** Ӳ�������ƿ��ƽṹ*/
typedef struct _ST_LED_CONTROL {
    /**  0��ʾ��ɫLED��1��ʾ��ɫLED���ο��꣺LEX_COLOR_XXX  */
    SS_UINT32   index;
    /**  0�����رգ�1�����򿪣� 2������˸���ο��꣺LED_STATE_XXX */
    SS_UINT32   state;
    /**  LED����˸ʱ���������룩*/
    SS_UINT32   interval;   
} ST_LED_CONTROL;

/** Virbox���ɷ��� ��Ϣ�����ص���������ǰ�汾 SLM_CALLBACK_VERSION02 ) ������lparam��ʱû��ʹ��*/
typedef SS_UINT32 (SSAPI *SS_CALL_BACK)(
    SS_UINT32   message,
    void*       wparam,
    void*       lparam
    );
//============================================================
//   Virbox���ɷ��� �ص���Ϣ message ����
//============================================================
/** �ص���Ϣ���ͣ���Ϣ��ʾ*/
#define SS_ANTI_INFORMATION			0x0101
/** �ص���Ϣ���ͣ����棨�ص�����wparam�����������ͣ�*/
#define SS_ANTI_WARNING				0x0102
/** �ص���Ϣ���ͣ��쳣*/
#define SS_ANTI_EXCEPTION			0x0103
/** �ص���Ϣ���ͣ��ݱ���*/
#define SS_ANTI_IDLE				0x0104

/** �ص������ͣ���������*/
#define SS_MSG_SERVICE_START        0x0200
/** �ص������ͣ�����ֹͣ*/
#define SS_MSG_SERVICE_STOP         0x0201
/** �ص������ͣ������ã��������� Virbox���ɷ��� ����ʱ���ѳ�ʼ����ɣ����ص�����wparam ��������*/
#define SS_MSG_LOCK_AVAILABLE       0x0202
/** �ص������ͣ�����Ч�����Ѱγ������ص�����wparam ��������*/
#define SS_MSG_LOCK_UNAVAILABLE     0x0203


//============================================================
//   �ص���Ϣ���ͣ���������SS_ANTI_WARNING��ֵ wparam ����
//============================================================
/** ��⵽�в�������ע�뵽Ŀ������У�һ��ָ�������Ĳ�����̬����ļ������ڵ�ǰ���̿ռ�*/
#define SS_ANTI_PATCH_INJECT		0x0201
/** ϵͳģ���쳣�����ܴ��ڽٳ���Ϊ����һЩλ��ϵͳĿ¼�Ķ�̬�⣨hid.dll��lpk.dll���ȣ��Ӵ����Ŀ¼����*/
#define SS_ANTI_MODULE_INVALID		0x0202
/** ��⵽���򱻵��������ӵ���Ϊ����ͨ��Ollydbg��Windbg�Գ�����и��ӵ���*/
#define SS_ANTI_ATTACH_FOUND		0x0203
/** ��Ϊ����Ч���̡߳���δ���壬����*/
#define SS_ANTI_THREAD_INVALID		0x0204
/** ��⵽�������߳�����״̬�쳣�����ܱ����⹥�������̱߳��ֶ������*/
#define SS_ANTI_THREAD_ERROR		0x0205
/** ��⵽��ģ�����ݵ�CRC��֤ʧ�ܣ����ܴ��ڶ��������Ϊ���Խ����е�ģ�����öϵ��۸ĵ�*/
#define SS_ANTI_CRC_ERROR			0x0206
/** ��⵽�������л������е��������ڣ���Ollydbg��Windbg*/
#define SS_ANTI_DEBUGGER_FOUND		0x0207
//=============================================================

/** ʱ��У׼��������ӳ���*/
#define SLM_FIXTIME_RAND_LENGTH     8

/** SS_CALL_BACK�İ汾 ��֧�ֿ�����API����İ汾��*/
#define SLM_CALLBACK_VERSION02      0x02

/** �ڴ��й���󳤶ȣ��ֽڣ�*/
#define  SLM_MEM_MAX_SIZE           2048

/** ����ִ�У�������뻺������С���ֽڣ�*/
#define SLM_MAX_INPUT_SIZE          1758
/** ����ִ�У���������������С���ֽڣ�*/
#define SLM_MAX_OUTPUT_SIZE         1758

/** �ӽ�����󻺳�����󳤶ȣ��ֽڣ�*/
#define SLM_MAX_USER_CRYPT_SIZE     1520

/** �û���֤������󳤶ȣ��ֽڣ�*/
#define SLM_MAX_USER_DATA_SIZE      2048

/** �û�������д����󳤶ȣ��ֽڣ�*/
#define SLM_MAX_WRITE_SIZE          1904

/** ����Ӳ�����豸˽Կǩ��������ǰ׺����slm_sign_by_device*/
#define SLM_VERIFY_DEVICE_PREFIX    "SENSELOCK"

/** ����Ӳ�����豸˽Կǩ�������ݴ�С����slm_sign_by_device*/
#define SLM_VERIFY_DATA_SIZE        41

/** Ӳ�����豸оƬ�ŵĳ��ȣ��������ֽڣ�*/
#define SLM_LOCK_SN_LENGTH          16

/** ������ID���ȣ��������ֽڣ�*/
#define SLM_DEVELOPER_ID_SIZE       8

/** ָ����¼������������������󳤶ȣ��ַ�����*/
#define SLM_MAX_SERVER_NAME         32

/** ָ����¼�����û�token��󳤶ȣ��ַ�����*/
#define SLM_MAX_ACCESS_TOKEN_LENGTH 64

/** ָ����¼������������ַ��󳤶ȣ��ַ�����*/
#define SLM_MAX_CLOUD_SERVER_LENGTH 100

/** ��Ƭ�������ӳ��ȣ��������ֽڣ�*/
#define SLM_SNIPPET_SEED_LENGTH     32

/** ������API���볤�ȣ��������ֽڣ�*/
#define SLM_DEV_PASSWORD_LENGTH     16

/** �û�GUID��󳤶ȣ��ַ����� */
#define SLM_CLOUD_MAX_USER_GUID_SIZE 	        128	

/** һ���GUID���ȣ������� */
#define SLM_GUID_LENGTH 	                16	

/** Ӳ���������ļ����ͣ������ļ�*/
#define SLM_FILE_TYPE_BINARY                    0
/** Ӳ���������ļ����ͣ���ִ���ļ��ļ�*/
#define SLM_FILE_TYPE_EXECUTIVE                 1
/** Ӳ���������ļ����ͣ���Կ�ļ�*/
#define SLM_FILE_TYPE_KEY                       2

/** Ӳ���������ļ����ԣ������߿ɶ�*/
#define SLM_FILE_PRIVILEGE_FLAG_READ            0x01
/** Ӳ���������ļ����ԣ������߿�д*/
#define SLM_FILE_PRIVILEGE_FLAG_WRITE           0x02
/** Ӳ���������ļ����ԣ������߿�ʹ�ã���Կ�ļ��� */
#define SLM_FILE_PRIVILEGE_FLAG_USE             0x04
/** Ӳ���������ļ����ԣ������߿�Զ������*/
#define SLM_FILE_PRIVILEGE_FLAG_UPDATE          0x08

/** Ӳ���������ļ����ԣ��ɶ�*/
#define SLM_FILE_PRIVILEGE_FLAG_ENTRY_READ      0x10
/** Ӳ���������ļ����ԣ���˼��д*/
#define SLM_FILE_PRIVILEGE_FLAG_ENTRY_WRITE     0x20
/** Ӳ���������ļ����ԣ���˼��ʹ�ã���Կ�ļ���*/
#define SLM_FILE_PRIVILEGE_FLAG_ENTRY_USE       0x40
/** Ӳ���������ļ����ԣ���˼��Զ������*/
#define SLM_FILE_PRIVILEGE_FLAG_ENTRY_UPDATE    0x80


/** ���ɵ�¼����ڴ���������Ҫ�ͻ�ά���ڴ棬Virbox���ɷ��� �濪����ά���ڴ棬����һ���ڴ�id����Χ��0-N֮��*/
typedef unsigned int SLM_HANDLE_INDEX; 

/** ���ɵ�¼ģʽ���Զ�ѡ��*/
#define SLM_LOGIN_MODE_AUTO             0x0000
/** ���ɵ�¼ģʽ��ָ����¼������*/
#define SLM_LOGIN_MODE_LOCAL            0x0001
/** ���ɵ�¼ģʽ��ָ����¼������ */
#define SLM_LOGIN_MODE_REMOTE           0x0002
/** ���ɵ�¼ģʽ��ָ����¼���� */
#define SLM_LOGIN_MODE_CLOUD            0x0004
/** ���ɵ�¼ģʽ��ָ����¼����*/
#define SLM_LOGIN_MODE_SLOCK            0x0008

/** ���ɵ�¼���ƣ��������е�����������ֶ�����������򲻵�¼���ṩѡ�񣬷����ҵ�������������ֱ�ӵ�¼��������ʹ�ô˱�־��*/
#define SLM_LOGIN_FLAG_FIND_ALL         0x0001    
/** ���ɵ�¼���ƣ�ָ�����ɰ汾*/
#define SLM_LOGIN_FLAG_VERSION          0x0004
/** ���ɵ�¼���ƣ�ָ�����ţ�оƬ�ţ�*/
#define SLM_LOGIN_FLAG_LOCKSN           0x0008
/** ���ɵ�¼���ƣ�ָ��������*/
#define SLM_LOGIN_FLAG_SERVER           0x0010
/** ���ɵ�¼���ƣ�ָ����Ƭ����*/
#define SLM_LOGIN_FLAG_SNIPPET          0x0020


/** �������ѯ������������ID����������*/
#define LANGUAGE_CHINESE_ASCII                  0x0001
/** �������ѯ������������ID��Ӣ��*/
#define LANGUAGE_ENGLISH_ASCII                  0x0002
/** �������ѯ������������ID����������*/
#define LANGUAGE_TRADITIONAL_CHINESE_ASCII      0x0003

/** ��ʼ��ʱ���ã���ʾ���յ� Virbox���ɷ��� ����Ϣ֪ͨ*/
#define SLM_INIT_FLAG_NOTIFY            0x01


/** MAC ��ַ���� */
#define SLM_MAC_SIZE                    6
/** CPU ��Ϣ��󳤶� */
#define SLM_MAX_CPU_INFO_SIZE           128
/** BIOS ��Ϣ��󳤶� */
#define SLM_MAX_BIOS_INFO_SIZE          128
/** Session ID ����󳤶ȣ��ַ���*/
#define SLM_MAX_SESSION_ID_SIZE         32


/** ��ʼ������   */
typedef struct _ST_INIT_PARAM {
    /** �汾���������ݣ���ǰʹ�� SLM_CALLBACK_VERSION02 */
    SS_UINT32       version;
    /** �����Ҫ���� Virbox���ɷ��� ֪ͨ���� SLM_INIT_FLAG_NOTIFY */
    SS_UINT32       flag;
    /** �ص�����ָ��*/
    SS_CALL_BACK    pfn;
    /** ͨ�����ӳ�ʱʱ�䣨���룩�������0����ʹ��Ĭ�ϳ�ʱʱ�䣨7�룩*/
    SS_UINT32       timeout;
    /** API���룬�ɴ� VirboxLM �ƿ��������ģ�https://developer.lm.virbox.com����ͨ�����鿴��������Ϣ����ȡ*/
    SS_BYTE         password[SLM_DEV_PASSWORD_LENGTH];
} ST_INIT_PARAM;

/** ����Login �ṹ*/
typedef struct _ST_LOGIN_PARAM{ 
    /** �ṹ���С�����*/
    SS_UINT32       size;
    /** Ҫ��¼������ID*/
    SS_UINT32       license_id;
    /** ���ɻỰ�ĳ�ʱʱ�䣨��λ���룩,��0��ʹ��Ĭ��ֵ��600��   */
    SS_UINT32       timeout;
    /** ���ɵ�¼��ģʽ��������������������������������SLM_LOGIN_MODE_XXX)�������0����ʹ��SLM_LOGIN_MODE_AUTO*/
    SS_UINT32       login_mode;
    /** ���ɵ�¼�ı�־����SLM_LOGIN_FLAG_XXX����������;�������ô˲��� */
    SS_UINT32       login_flag;
    /** ���ɵ�¼ָ������Ψһ���кţ���ѡ��*/
    SS_BYTE         sn[SLM_LOCK_SN_LENGTH];
    /** ��������������ַ����ѡ������ʶ��IP��ַ */
    SS_CHAR         server[SLM_MAX_SERVER_NAME];
    /** �����û�token����ѡ��*/
    SS_CHAR         access_token[SLM_MAX_ACCESS_TOKEN_LENGTH];
    /** ������������ַ����ѡ��*/
    SS_CHAR         cloud_server[SLM_MAX_CLOUD_SERVER_LENGTH];
    /** ��Ƭ�������ӣ���ѡ�������Ҫ֧����Ƭ����,login_flag��Ҫָ��ΪSLM_LOGIN_FLAG_SNIPPET*/
    SS_BYTE         snippet_seed[SLM_SNIPPET_SEED_LENGTH];
    /** �ѵ�¼�û���guid����ѡ�� */
    SS_CHAR         user_guid[SLM_CLOUD_MAX_USER_GUID_SIZE];
} ST_LOGIN_PARAM;

/** ����Ϣ�ṹ  */
typedef struct _ST_LOCK_INFO {
    SS_UINT32           size;               //  �ṹ���С
    SS_UINT8            device_type;        //  �豸���ͣ�0 = ��ͨ�û�����2 = ʱ���û���
    SS_UINT8            device_mode;        //  �豸�ͺ�, 0 = ����5�û���
    SS_UINT16           res;                //  ����
    SS_UINT8            lmfirm_version[4];  //  Entry�̼��汾
    SS_UINT32           h5firm_version;     //  H5�̼��汾
    SS_UINT32           hardware_version;   //  Ӳ���汾
    SS_UINT32           manufacture_date;   //  ��������
    SS_UINT32           total_space;        //  �ܹ��ռ�
    SS_UINT32           available_space;    //  ʣ��ռ�
    SS_UINT32           protocol;           //  ͨѶЭ��
    SS_UINT32           slave_addr;         //  ���ߵ�ַ
    SS_UINT32           clock;              //  ����ʱ�ӣ�UTC��
    SS_UINT32           clock_type;         //  0 = ����ʱ�ӣ�2 = Ӳ��ʱ��
    SS_UINT32           drop_times;         //  ʱ�����������
    SS_BYTE             lock_sn[SLM_LOCK_SN_LENGTH];    //  ����
    SS_BYTE             user_info[128];     //  ���ı��������û��������洢����Ǳ��
    SS_BYTE             inner_info[128];    //  ���ı��������ڲ�����
} ST_LOCK_INFO;

typedef struct _ST_SESSION_INFO {
    SS_UINT32           size;                     //  �����С
    SS_UINT32           license_id;               //  �Ѿ���¼������id 
    union
    {
        SS_BYTE         sn[SLM_LOCK_SN_LENGTH];
        SS_BYTE         user_guid[SLM_GUID_LENGTH];
    };
    SS_UINT32           app_time_out;          //  ���������õ�time_out
    SS_UINT32           app_process_id;        //  ����id
    SS_UINT8            mac[SLM_MAC_SIZE];        //  ����MAC
    char                session_id[SLM_MAX_SESSION_ID_SIZE];
    char                cpuinfo[SLM_MAX_CPU_INFO_SIZE];   //  CPU ID Verndor ��Ϣ
    char                bios[SLM_MAX_BIOS_INFO_SIZE];     //  BIOS��Ϣ �������Ƶ�   
} ST_SESSION_INFO;

typedef struct _ST_SLM_LICENSE_INFO{
    SS_UINT32   size;
    SS_UINT32   lic_ver;        //  ���ɰ汾��
    SS_BYTE     flag[4];        //  ���ɱ�־λ��ע����Ҫ�Ķ�flag��ƫ�ƣ�
    SS_BYTE     guid[SLM_LOCK_SN_LENGTH];       //  ���ɵ�guid
    SS_UINT32   lic_id;         //  ����ID
    SS_UINT32   start_time;     //  ��ʼʱ��
    SS_UINT32   end_time;       //  ��ֹʱ��
    SS_UINT32   span_time;      //  ʱ����
    SS_UINT32   time_stamp;     //  ����ʱ���
    SS_UINT16   serial;         //  ������ˮ��
    SS_UINT16   concurrency;    //  ������󲢷���
    SS_UINT32   count;          //  ����������
    SS_UINT64   module_bit;     //  ģ�鹦�ܱ�ʶλ
    SS_UINT32   lock_time;      //  ����ʱ��
    SS_UINT32   first_use_time; //  ��һ��ʹ��ʱ��
    SS_UINT32   curr_count;     //  ��ǰʣ�����
    SS_UINT32   offline_period; //  ������ʱ��
    SS_UINT32   bind_node;      //  ����ͬʱʹ�õĻ���������������޹أ�ֻ�������й�
    SS_UINT32   max_bind_node;  //  ���ɰ󶨵Ļ���������������йأ�һ���󶨣��������
    SS_UINT32   raw_size;   //  ��д����С
    SS_UINT32   rom_size;   //  ֻ������С
    SS_UINT32   pub_size;   //  ��������С
} ST_SLM_LICENSE_INFO;

typedef struct _ST_FILE_INFO {
    SS_BYTE     validate;                /** ��־��Щ����Ч�����������ļ�������ʹ��              */
    SS_BYTE     type;                    /** �ļ����ͣ������޸�                                 */
    SS_UINT16   pri;                     /** �ļ�����Ȩ��                                       */
    SS_UINT32   size;                    /** �ļ���С                                           */
    SS_UINT32   time;                    /** �ļ��޸�ʱ��                                       */
    SS_BYTE     name[16];                /** �ļ���  ��ǰ�ļ����Ƴ���16�ֽ�                     */
} ST_FILE_INFO;

typedef struct _ST_FILE_INFO_LIST{
    SS_UINT32       version;        // ��ǰΪ1
    SS_UINT32       count;
    ST_FILE_INFO    info[1];
} ST_FILE_INFO_LIST;

typedef enum _ENUM_DESC_TYPE {
    ENUM_DESC_LOCAL = 0,
    ENUM_DESC_REMOTE = 1,
    ENUM_DESC_CLOUD = 2,
    ENUM_DESC_SMART_OFFLINE = 3,
    ENUM_DESC_SLOCK = 4,
} ENUM_DESV_TYPE;
//  �豸����
typedef struct _ST_DEV_INFO {
    SS_UINT16  		    desc_type;      //	local, remote or cloud.(DESC type)
    SS_UINT16           firm_type;      //  LM���ͣ�LM_FIRM_TYPE_XXX)
    SS_BYTE 	        developer_id[SLM_DEVELOPER_ID_SIZE];
    union {
        struct {
            SS_CHAR         cloud_server[114];
            SS_CHAR         user_token[48];
            SS_BYTE         user_guid[SLM_GUID_LENGTH];
        } cloud, smart_offline;  //  ����/��������

        struct {
            SS_BYTE         serial[SLM_LOCK_SN_LENGTH];
            SS_CHAR         ip[64];
            SS_UINT16       port;
            SS_CHAR         host_name[SLM_MAX_SERVER_NAME];
            //  reserved    
        } h5;  //  Ӳ����

        struct {
            SS_BYTE         user_guid[SLM_GUID_LENGTH];
            //SS_CHAR         ip[SS_IP_MAX_SIZE];
            //SS_UINT16       port;
            SS_BYTE         reserved[114 - 16 - 32];
            SS_CHAR         host_name[SLM_MAX_SERVER_NAME];
            // reserved for remote slock
            SS_CHAR         account_name[64];
        } slock;  //  ����
    };

} ST_DEV_INFO;

typedef struct _ST_ENUM_DEVICE {
    ST_DEV_INFO     dev_info;
    ST_LOCK_INFO    lock_info;
} ST_ENUM_DEVICE;

typedef struct _ST_DEV_INFO_LIST{
    SS_UINT32       version;        // ��ǰΪ1
    SS_UINT32       count;
    ST_ENUM_DEVICE  info[1];
} ST_DEV_INFO_LIST;

typedef struct _ST_LICENSE_IDS{
    SS_UINT32       count;
    SS_UINT32       lic_ids[1];
} ST_LICENSE_IDS;

/** 
*   @defgroup group1_lm_runtime  Virbox Runtime Virbox�û�����ʱ��
*   This is Virbox lm_runtime API Virbox�û�����ʱ��
*   @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/*!
*   @brief      Runtime API��ʼ����������������Runtime API�����ȵ��ô˺������г�ʼ��
*   @param[in]  pst_init    ��ʼ����������ST_INIT_PARAM�ṹ����
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    slm_init������ Virbox Runtime API �ĳ�ʼ����������ʹ������ Virbox Runtime API֮ǰ������Ҫ�ȵ��ô˺�����
                ��ʼ��������Ҫ�Ƕ�Runtime�����ĳ�ʼ�����������������Ի��ơ�
*   @see        ST_INIT_PARAM slm_cleanup
*/
SS_UINT32 SSAPI slm_init(IN ST_INIT_PARAM* pst_init);

/*!
*   @brief      ����������Ϣ(����Ӳ������Ч)
*   @param[in]  license_id      Ҫ���ҵ�����ID��0--0xFFFFFFFF��
*   @param[in]  format          ������ʽ���ο�INFO_FORMAT_TYPE��Ŀǰ��֧��JSON��STRUCT
*   @param[out] license_desc    ����������Ϣ��ָ�룬��ʽ��formatָ���������slm_free�ͷ�
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    �˺������ڲ��ұ���������������������Ϣ������ɹ�����Ҫ����slm_free �ͷ�license_desc��
*               �� format = STRUCT ʱ license_desc ������ �ο� ST_DEV_INFO_LIST
*   @see        INFO_FORMAT_TYPE
*/
SS_UINT32 SSAPI slm_find_license(
    IN  SS_UINT32           license_id,
    IN  INFO_FORMAT_TYPE    format,
    OUT char**              license_desc
    );


/*!
*   @brief      ö���ѵ�¼���û�token
*   @param[out] access_token    Ĭ���û���token�������slm_free�ͷ�
*   @return     �ɹ�����SS_OK�����oauth��̨����δ�������򷵻�SS_ERROR_BAD_CONNECT
*   @remarks    ����ɹ�����Ҫ����slm_free �ͷ� access_token
*/
SS_UINT32 SSAPI slm_get_cloud_token(OUT SS_CHAR** access_token);

/*!
*   @brief      ��ȫ��¼����,��JSON���ݲ���,���Ҽ��ʱ�䣨�Ƿ��ڻ����Ƿ����ڿ�ʼʱ�䣩���������������Ƿ���㣬
*               ����м��������������Լ��ٶ�Ӧ�ļ���������������������ʱ�������粢����������
*   @param[in]  license_param ��¼��������������������������licenseid������ָ����¼���Եȡ�
*   @param[in]  param_format  ���������ַ������ͣ���֧��STRUCT��Ϊȷ����ȫ��SDK�汾��2.1.0.15128֮������ɵ�¼������֧��json��¼�ķ�����
*   @param[out] slm_handle    ���ص�¼֮�����ɾ��indexֵ,��Χ��0-256֮�䡣
*   @param[out] auth          ��֤login���������Ƿ���ȷ������ECCǩ���ͷ���ֵ���ܣ���ʹ�ÿ�����NULL��
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks
*   - slm_login������ϸ˵����
*       -# ���Զ����ҿ����������ɾ����
*       -# ��runtime�������������ڴ�������߳���Ϣ��
*       -# �����������Ҫ���ڼ�ػỰ���̣���������������߱������Լ��ͷŶ�Ӧ���ڴ��������Դ��
*       -# LM�����ڿͻ������Զ����룬����RSA ��Կ����֤�豸ID�������̱�ŵ�һ����֤�ֶΡ�
*       -# LM�����������붼Ҫlogin֮�������Ȩ�޲��� �����д���ӽ��ܵȲ�����
*   - ���ܲ���˵����
*       - ���ýṹ�壺
*       -# �ο� ST_LOGIN_PARAM �ṹ��ϸ����
*       -# size         �����ṹ���С�����������Ϊ����ṹ��Ĵ�С������᷵�ش���SS_ERROR_RUNTIME_VERSION
*       -# license_id   ����ID��Ϊ32λ��������ȡֵ��Χ0-4294967295����д����
*       -# login_mode   ���ɵ�¼ģʽ����Ϊ�Զ�ģʽ���Լ��������������������������������
*       -# sn           ָ����¼���� Ϊ������Ӳ����оƬ�ţ�16�ֽڣ���
*       -# access_token �����¼����������Ҫָ��ͨ��oauth��֤��access token
*       -# time_out     ָ����¼�Ự��ʱ ��λΪ�롣�������д��Ĭ��Ϊ600�룬Ӳ��������������Ĭ����С������60s����󲻽��ó���12Сʱ��12 * 60 * 60 �룩��
*       -# user_guid    ��¼�û���guid����󳤶�ΪSLM_CLOUD_MAX_USER_GUID_SIZE��һ�㲻��д
*   @code
*       {
*           SS_UINT32           status = SS_OK;
*           ST_INIT_PARAM       init_param = { SLM_CALLBACK_VERSION02, 0, 0, 20000 };
*           SLM_HANDLE_INDEX    slm_handle = 0;
*           ST_LOGIN_PARAM      login_param = { 0 };
*           
*           // psd�Ǳش�������ÿ��������˽�У�����й¶����� VirboxLM �ƿ��������Ļ�ȡ��https://developer.lm.virbox.com
*           const char          psd[] = { 0xDB, 0x3B, 0x83, 0x8B, 0x2E, 0x4F, 0x08, 0xF5, 0xC9, 0xEF, 0xCD, 0x1A, 0x5D, 0xD1, 0x63, 0x41};
*
*           memcpy(init_param.password, psd, sizeof(init_param.password));  
*
*           status = slm_init(&init_param);
*           if(status != SS_OK)
*           {
*               return ;
*           }
*
*           login_param.license_id = 0;
*           login_param.size = sizeof(ST_LOGIN_PARAM);      // �ṹ�峤�ȱ��븳ֵΪ��ȷ�ĳ��ȣ���������
*           login_param.login_mode = SLM_LOGIN_MODE_LOCAL;
*
*           status = slm_login(&login_param, STRUCT, &slm_handle, NULL);
*           if(status != SS_OK)
*           {
*               //todo do: deal error code
*               return ;
*           }
*
*           slm_logout(slm_handle);
*       }
*   @endcode
*   @see slm_logout SS_UINT32 INFO_FORMAT_TYPE ST_LOGIN_PARAM
*/
SS_UINT32 SSAPI slm_login(
    IN  const ST_LOGIN_PARAM*  license_param,
    IN  INFO_FORMAT_TYPE       param_format,
    OUT SLM_HANDLE_INDEX *     slm_handle,
    IN OUT void*               auth
    );

/*!
*   @brief      ���ɵǳ��������ͷ����ɾ������Դ
*   @param[in]  slm_handle    ���ɾ��ֵ����slm_login�õ�
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ����ʹ������ʱ����Ҫ����slm_logout�˳���¼���ɣ�������ռ��Runtime���е��ڴ����Դ��
*               ���磬����Runtime ��ֻ֧�����256����¼�������ֻ��¼���ɶ����ǳ����ɣ�һ������256����¼�㽫ռ������Runtime���е���Դ�����º����ĵ�¼ʧ��
*   @see        slm_login
*/
SS_UINT32 SSAPI slm_logout(SLM_HANDLE_INDEX slm_handle);

/*!
*   @brief      ���ֵ�¼�Ự�����������Ϊ����ʬ�������
*   @param[in]  slm_handle    ���ɾ��ֵ
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ��ʬ��������������ѭ�����߱�����slm_handle�����Զ����ͷţ����Runtime�����ڴ����Դ�˷�
*   @code
*       DWORD WINAPI __stdcall _ThreadKeepalive(void *pVoid)
*       {
*           SLM_HANDLE_INDEX slm_handle = *(SLM_HANDLE_INDEX *)(pVoid);
*           SS_UINT32        status = SS_OK;
*
*           while (1)
*           {
*               status = slm_keepalive(slm_handle);
*               if(status != SS_OK)
*               {
*                   //todo do: deal error code
*               }
*               Sleep(1000 * 10);      // ʮ���ӽ���һ���������ӣ���֤�Ự����Ч��
*           }
*       }
*       
*       {
*           SS_UINT32           status = SS_OK;
*           SLM_HANDLE_INDEX    slm_handle = 0;
*           ST_LOGIN_PARAM      login_param = { 0 };
*           HANDLE hThread;
*           DWORD  id = 0;
*
*           login_param.license_id = 0;
*           login_param.size = sizeof(ST_LOGIN_PARAM);
*           login_param.login_mode = SLM_LOGIN_MODE_LOCAL;
*           login_param.time_out = 30;    // ���ûỰΪ30�볬ʱ
*
*           status = slm_login(&login_param, STRUCT, &slm_handle, NULL);
*           if(status != SS_OK)
*           {
*               //todo do: deal error code
*               return ;
*           }
*
*           hThread = CreateThread(NULL, 0, _ThreadKeepalive, &slm_handle, 0, &id);
*           if (hThread == NULL)
*           {
*               //todo: deal error
*           }
*       }
*   @endcode
*   @see       slm_login
*/
SS_UINT32 SSAPI slm_keep_alive(SLM_HANDLE_INDEX slm_handle);

/*!
*   @brief      ���ɼ��ܣ���ͬ������ID��ͬ�Ŀ����߼��ܽ����ͬ
*   @param[in]  slm_handle   ���ɾ��ֵ����slm_login�õ�
*   @param[in]  inbuffer     �������뻺����,��Ҫ16�ֽڶ��룬����ܳ��� 1520 �ֽڡ�
*   @param[out] outbuffer    �������������,��Ҫ16�ֽڶ��� 
*   @param[in]  len          ���ܻ�������С��Ϊ16����������
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ����Ӳ����������ֽ������ޣ���˼ӽ���ʱһ�δ�����ֽڲ��ó���1520���ֽڣ�����Ҫ�ӽ��ܵ��ֽ������ڴ�������ƣ�
*               �ɲ��ý�ȡ��������ѭ�����õķ�ʽ���мӽ��ܣ��ӽ��ܽ�������ܵ��κ�Ӱ�졣            
*   @code
*       {
*           SS_UINT32   status = SS_OK;
*           SS_BYTE     plain[32] = { 0 };  // �ӽ���ʹ��AES�ԳƼ��ܣ��������ݱ���16�ֽڶ���
*           SS_BYTE     enc[32] = { 0 };
*           SS_BYTE     dec[32] = { 0 };
*
*           memcpy(data, "test data...", strlen("test data..."));
*
*           status = slm_encrypt(slm_handle, plain, enc, sizeof(enc));
*           if(status != SS_OK)
*           {
*               // todo: deal error code
*               return ;
*           }
*
*           status = slm_decrypt(slm_handle, enc, dec, sizeof(dec));
*           if(status != SS_OK)
*           {
*               // todo: deal error code
*               return ;
*           }
*
*           //�ԱȽ��ܺ�������Ƿ���������
*           //if(plain == dec)
*           //{ 
*           //    SUCCESS;
*           //}
*           //else
*           //{
*           //    FAILURE;
*           //}
*       }
*   @endcode
*   @see        slm_encrypt slm_decrypt slm_login
 */
SS_UINT32 SSAPI slm_encrypt(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_BYTE*            inbuffer,
    OUT SS_BYTE*            outbuffer,
    IN  SS_UINT32           len
    );

/*!
*   @brief      ���ɽ��ܣ���ͬ������ID��ͬ�Ŀ����߼��ܽ����ͬ
*   @param[in]  slm_handle   ���ɾ��ֵ����slm_login�õ�
*   @param[in]  inbuffer     �������뻺����,��Ҫ16�ֽڶ��롣����ܳ��� 1520 �ֽڡ�
*   @param[out] outbuffer    �������������,��Ҫ16�ֽڶ��� 
*   @param[in]  len          ���ܻ�������С��Ϊ16����������
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ���ܷ�ʽ����AES�ԳƼ��ܣ���Կ�ڼ������ڣ�ָӲ��������������������ͬ�����ɣ���û���κλ����ܳ������ڱ�֤Ч�ʵ�ͬʱ��Ҳ��󻯵ļ�ǿ�˰�ȫ�ԡ�
*               ����Ӳ����������ֽ������ޣ���˼ӽ���ʱһ�δ�����ֽڲ��ó���1520���ֽڣ�����Ҫ�ӽ��ܵ��ֽ������ڴ�������ƣ��ɲ��ý�ȡ��������ѭ������
*               �ӿڵķ�ʽ���мӽ��ܣ��ӽ��ܽ�������ܵ��κ�Ӱ�졣
*   @code
*       //see slm_encrypt
*   @endcode
*   @see        slm_encrypt slm_login
 */
SS_UINT32 SSAPI slm_decrypt(
    IN  SLM_HANDLE_INDEX    slm_handle, 
    IN  SS_BYTE*            inbuffer, 
    OUT SS_BYTE*            outbuffer, 
    IN  SS_UINT32           len
    );

/*!
*   @brief      ������ɵ��û���������С
*   @param[in]  slm_handle ���ɾ��ֵ����slm_login�õ�
*   @param[in]  type       �û����������ͣ����Ͷ���� LIC_USER_DATA_TYPE
*   @param[out] pmem_size  �����û���������С
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @see        LIC_USER_DATA_TYPE slm_user_data_getsize slm_user_data_read slm_user_data_write
 */
SS_UINT32 SSAPI slm_user_data_getsize(
    IN SLM_HANDLE_INDEX     slm_handle,
    IN LIC_USER_DATA_TYPE   type,
    OUT SS_UINT32*          pmem_size
    );

/*!
*   @brief      ���������ݣ����Զ�ȡPUB��RAW��ROM
*   @param[in]  slm_handle   ���ɾ��ֵ����slm_login�õ�
*   @param[in]  type         �û����������ͣ��ο�LIC_USER_DATA_TYPE
*   @param[out] readbuf      �û���������ȡ������
*   @param[in]  offset       ��ȡ���û�������������ƫ��
*   @param[in]  len          �ⲿʹ�õĴ洢��������С
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    �����������֧��64k���ݵĶ�ȡ��
*   @code
*       {        
*           SS_UINT32 size = 0;
*           SS_BYTE   *buff = 0;
*           SS_UINT32 status = SS_OK;
*       
*           status = slm_user_data_getsize(slm_handle, ROM, &size);   // ��ȡֻ��������
*           if (status == SS_OK && size > 0)
*           {
*               buff = (SS_BYTE *)calloc(sizeof(SS_BYTE), size);
*               status = slm_user_data_read(slm_handle, ROM, buff, 0, size);
*               if(status != SS_OK)
*               {
*                   // todo: deal error code
*               }
*               // ���ڴ˴�����ȡ����ֻ��������
*               free(buff);
*               buff = 0;
*           }
*       }
*   @endcode
*   @see        LIC_USER_DATA_TYPE slm_user_data_getsize slm_user_data_read slm_user_data_write
 */
SS_UINT32 SSAPI slm_user_data_read(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  LIC_USER_DATA_TYPE  type,
    OUT SS_BYTE*            readbuf,
    IN  SS_UINT32           offset,
    IN  SS_UINT32           len
    );

/*!
*   @brief      д���ɵĶ�д������ ,����������֮ǰ����ȷ�����ڶ�д���Ĵ�С������ʹ��slm_user_data_getsize���
*   @param[in]  slm_handle      ���ɾ��ֵ����slm_login�õ�
*   @param[in]  writebuf        Ҫд����������ݴ���
*   @param[in]  offset          ����������������ƫ�ƣ���������������д��λ��
*   @param[in]  len             Ҫд�����ݵĳ��ȣ�������󳤶� = min(slm_user_data_getsize, SLM_MAX_WRITE_SIZE)
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ����д������ͨ��Ӧ�ó���д�����ݣ���˴˽ӿڲ���Ҫ�������������ͣ��ӿڻ�ֱ�ӽ�����д���д����
*               ������д��ʱ��������ݳ��ȴ��� SLM_MAX_WRITE_SIZE ����Ҫ���������н��зְ�д�롣
*   @code
*       {
*           SS_UINT32 size = 0;
*           SS_BYTE   write[20] = { "write data" };
*           SS_UINT32 status = SS_OK;
*           SS_UINT32 offset = 0
*
*           status = slm_user_data_getsize(slm_handle, RAW, &size);   // ����д����д������
*           if (status == SS_OK && size > 0)
*           {
*               size = min( offset + sizeof(write), size);    // д�����ݲ��ó�����ȡ�������ݳ���
*               status = slm_user_data_write(slm_handle, write, offset, size);
*               if(status != SS_OK)
*               {
*                   // todo: deal error code
*               }
*           }
*       }
*   @endcode
*   @see       LIC_USER_DATA_TYPE   slm_user_data_getsize slm_user_data_read slm_user_data_write
 */
SS_UINT32 SSAPI slm_user_data_write(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_BYTE*            writebuf,
    IN  SS_UINT32           offset,
    IN  SS_UINT32           len
    );

/*!
*   @brief      ���ָ�����ɵĹ�������������С����Ҫ��¼0������
*   @param[in]  slm_handle    0�����ɾ��ֵ���������ɾ���޷�ͨ���˷�ʽ�õ���������С����slm_login�õ�
*   @param[in]  license_id    Ҫ��ȡ��������С������ID
*   @param[out] pmem_size     ������������С
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ĳЩ����£��û�ϣ���ڲ���¼���ɵ�����£���ȡ�����ɵĹ��������ݣ����������Ѿ������ã����ɵ��ڡ������þ��ȣ���
*               ��ʱ�û�����ͨ�����ô˽ӿڽ��л�ȡ��ǰ������Ҫ��¼0�����ɣ�Ȼ��0�����ɾ����Ҫ��ѯ������������ID�����������ȡ�����ɵĹ��������ݡ�
*               ��ע���й�0�����ɵĽ��ܣ��ɲο���Virbox������ϵ�����ɹ������ĵ��й���0�����ɵ�����
*   @see        slm_user_data_read_pub 
*/
SS_UINT32 SSAPI slm_pub_data_getsize(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_UINT32           license_id,
    OUT SS_UINT32*          pmem_size
    );

/*!
*   @brief      ��ȡ���ɹ���������Ҫ��¼0������
*   @param[in]  slm_handle   0�����ɾ��ֵ���������ɾ���޷�ͨ���˷�ʽ�õ����������ݣ���slm_login�õ�
*   @param[in]  license_id   Ҫ��ȡ���������ݵ�����ID
*   @param[out] readbuf      �ⲿ�洢Ҫ��ȡ���������ݵĻ�����
*   @param[in]  offset       Ҫ��ȡ������������ƫ��
*   @param[in]  len          �ⲿ�洢��������С
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ĳЩ����£��û�ϣ���ڲ���¼���ɵ�����£���ȡ�����ɵĹ��������ݣ����������Ѿ������ã����ɵ��ڡ������þ��ȣ���
*               ��ʱ�û�����ͨ�����ô˽ӿڽ��л�ȡ��ǰ������Ҫ��¼0�����ɣ�Ȼ��0�����ɾ����Ҫ��ѯ������������ID�����������ȡ�����ɵĹ��������ݡ�
*               ��ע���й�0�����ɵĽ��ܣ��ɲο���Virbox������ϵ�����ɹ������ĵ��й���0�����ɵ�����
*   @code
*       {
*           SS_UINT32 size = 0;
*           SS_BYTE   *buff = 0;
*           SS_UINT32 status = SS_OK;
*           SS_UINT32 license_id = 1;   // ��Ҫ��ȡ������������ID
*
*           // �˴���Ҫ��¼������ɣ���ο� slm_login �� code ʾ��
*
*           status = slm_pub_data_getsize(slm_handle, license_id, &size);   // ��ȡָ�����ɵĹ���������
*           if (status == SS_OK && size > 0)
*           {
*               buff = (SS_BYTE *)calloc(sizeof(SS_BYTE), size);
*               status = slm_user_data_read(slm_handle, license_id, buff, 0, size);
*               if(status != SS_OK)
*               {
*                   // todo: deal error code
*               }
*               // ���ڴ˴�����ȡ��������
*               free(buff);
*               buff = 0;
*           }
*       }
*   @endcode
*   @see        slm_user_data_pub_getsize 
*/
SS_UINT32 SSAPI slm_pub_data_read(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_UINT32           license_id,
    OUT SS_BYTE*            readbuf,
    IN  SS_UINT32           offset,
    IN  SS_UINT32           len
    );

/*!
*   @brief      ��ȡ�ѵ�¼���ɵ�״̬��Ϣ������������Ϣ��Ӳ������Ϣ��
*   @param[in]  slm_handle  ���ɾ��ֵ����slm_login�õ�
*   @param[in]  type        ��Ϣ���ͣ��ο� INFO_TYPE
*   @param[in]  format      ��Ϣ��ʽ���ο� INFO_FORMAT_TYPE����ǰ��֧��JSON��STRUCT
*   @param[out] result      ���ؽ��������������سɹ�����Ҫ����slm_free�ͷ�
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ͨ�����ô˽ӿڣ����Ի�ȡ��ǰ��¼���ɵģ�������Ϣ���Ự��Ϣ��Ӳ������Ϣ����֧��Ӳ�������������ļ��б�����֧��Ӳ������
*   @code
*   // JSON ����˵����
*   - type = LICENSE_INFO������ֶβ����ڣ����ʾû�и��������ƣ�
*       {
*           "rom_size" :number(������֧�֣�
*           "raw_size" :number��������֧�֣�
*           "pub_size" :number��������֧�֣�
*           
*           "type": "local"/"remote"/"cloud"
*           "sn":"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
*           "guid":"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
*           "developer_id":   "xxxxxxxxxxxxxxxx"
*           "license_id":     number
*           "enable":         bool
*           "start_time":     number
*           "end_time":       number
*           "first_use_time": number
*           "span_time":      number
*           "counter":        number
*           "concurrent_type": "process" / "win_user_session"    // ��������
*           "concurrent":     number    //������
*           "version":        number
*           "module":number
*           "last_update_timestamp":  number
*           "last_update_timesn":     number
*           "lock_time":      number     // ���ɱ�������ʱ�䣬�������ɲ������ֶ�
*       }
*
*   - type = SESSION_INFO
*       {
*           "app_process_id":  number,
*           "app_time_out":    number,
*           "session_id":      ��ֻ֧��������
*           "bios": "BIOS information", ��������֧�֣�
*           "cpuinfo": "CPU name", ��������֧�֣�
*           "sn": ""(HEX16 String), ��������֧�֣�
*           "mac":"00-00-00-00-00-00" ��������֧�֣�
*       }
*    
*   - type = LOCK_INFO ��������֧�֣�
*       {
*           "avaliable_space": number(Bytes),
*           "communication_protocol": number,
*           "lock_firmware_version": "0.0.0.0",
*           "lm_firmware_version": "1.1.1.1",
*           "h5_device_type": number,
*           "hardware_version": "2.2.2.2",
*           "manufacture_date": "2000-01-01 00:00:00",
*           "lock_sn": ""(HEX16 String),
*           "slave_addr": number,
*           "clock": number(UTC Time),
*           "user_info": ""(HEX16 String)(128�ֽ�)
*           "inner_info": ""(HEX16 String)(128�ֽڣ�
*       }
*   
*   - type = FILE_LIST
*       [
*        {
*           "validate":0,       (��־��Щ����Ч�����ع���)
*           "type":0,           (�ļ����͡��������ļ�=0����ִ���ļ�(evx)=1����Կ�ļ�=2)
*           "privilege":0,      (�ļ�����Ȩ�ޣ����Ȩ��Ϊ 0xFF)
*           "size":123,         (�ļ���С����λ���ֽ�)
*           "time":12345678,    (�ļ�����ʱ�䣬UTCʱ����)
*           "name":"file1.evx"  (�ļ�����)
*        },
*        {...}
*       ]
*
*   // STRUCT ����˵��
*   - type = LICENSE_INFO
*      result �ο��ṹ ST_SLM_LICENSE_INFO
*   - type = SESSION_INFO
*       result �ο��ṹ ST_SESSION_INFO
*   - type = LOCK_INFO
*       result �ο��ṹ ST_LOCK_INFO
*   - type = FILE_LIST
*       result �ο��ṹ ST_FILE_INFO_LIST
*   @endcode
*   @see INFO_TYPE INFO_FORMAT_TYPE SLM_HANDLE_INDEX slm_login
*/
SS_UINT32 SSAPI slm_get_info(
    IN  SLM_HANDLE_INDEX	slm_handle,
    IN  INFO_TYPE			type,
    IN  INFO_FORMAT_TYPE	format,
    OUT char**              result
    );

/*!
*   @brief      �������Զ����㷨�����ڿ�ִ���㷨�����ھ�̬����ִ�У���֧��Ӳ��������
*               �ɿ�����d2cAPI d2c_add_pkgǩ����ִ�д���ӿ����ɣ���ǰ����ִ���㷨���ص�����
*   @param[in]  slm_handle  ���ɾ��ֵ����slm_login�õ�
*   @param[in]  exfname     ���ڿ�ִ���㷨���ļ���
*   @param[in]  inbuf       Ҫ�������ڿ�ִ���㷨�����ݣ����뻺����
*   @param[in]  insize      �������ݳ���(���֧�ֻ�������С�ο��꣺SLM_MAX_INPUT_SIZE)
*   @param[out] poutbuf     ���ڿ�ִ���㷨�ķ������ݣ����������
*   @param[in]  outsize     ������泤��
*   @param[out] pretsize    ʵ�ʷ��ػ��泤��(���֧�ֻ�������С�ο��꣺SLM_MAX_OUTPUT_SIZE)
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ���������̿��Դ�APP��������ֲ�ؼ����뵽���ڣ�������̸���ͳ����4�뾫��5û��̫������𡣣���ֲ�������������ھ���ϵ�е��Ͽͻ����������������̿��԰���������ҵ������߼����㷨��ֲ������������ִ�У���
*               ����������Ҫ�����ɰ󶨡�
*
*               ���ɰ󶨵ĺô���
*               -# �ͻ�����Ҫ�Լ�д�������ж���Ȩ�����
*               -# ��ֲ�Ĵ�����Խ������������Ȩ��͸����ʹ����ʱ���޴�ʧЧ���������䷽�㡣
*               -# ������ʧЧ֮�󣬸��㷨����ִ�У������������̶��������жϣ��ǳ���ȫ��
*
*   @code
*       {
*           SS_BYTE inbuff[MAX_BUFFER_SIZE] = { 0 };
*           SS_BYTE outbuff[MAX_BUFFER_SIZE] = { 0 };
*           char    *dongle_exe = "dongle.evx"          // ���ڿ�ִ�г����ļ���
*           SS_UINT32 retlen = 0;
*           SS_UINT32 status = 0;
*
*           memcpy(inbuff, "1234567890", 10);
*
*           status = slm_execute_static(slm_handle, dongle_exe, inbuff, 10, outbuff, MAX_BUFFER_SIZE, &retlen);
*           if(status != SS_OK)
*           {
*               //todo do: deal error code
*               return ;
*           }
*           // todo: �������ڳ���ķ�������
*       }
*   @endcode
*   @see        slm_login slm_execute_dynamic slm_snippet_execute
*/
SS_UINT32 SSAPI slm_execute_static(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  const char*         exfname,
    IN  SS_BYTE*            inbuf,
    IN  SS_UINT32           insize,
    OUT SS_BYTE*            poutbuf,
    IN  SS_UINT32           outsize,
    OUT SS_UINT32*          pretsize
    );

/*!
*   @brief      �������Զ����㷨�����ڿ�ִ���㷨�����ڶ�ִ̬�д��루��֧��Ӳ����������Ӧ�ó������й��������ص����ڽ���ִ��
*   @param[in]  slm_handle  ���ɾ��ֵ����slm_login�õ�
*   @param[in]  exf_buffer  ��̬exf�㷨���������ɿ�����d2cAPI gen_dynamic_code���ɵ�D2C����
*   @param[in]  exf_size    ��̬exf�ļ���С
*   @param[in]  inbuf       Ҫ�������ڿ�ִ���㷨�����ݣ����뻺����
*   @param[in]  insize      �������ݳ���(���֧�ֻ�������С�ο��꣺SLM_MAX_INPUT_SIZE)
*   @param[out] poutbuf     ���ڿ�ִ���㷨�ķ������ݣ����������
*   @param[in]  outsize     ������泤��
*   @param[out] pretsize    ʵ�ʷ��ػ��泤��(���֧�ֻ�������С�ο��꣺SLM_MAX_OUTPUT_SIZE)
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ������ slm_execute_static ���ھ�̬����ִ�С����ھ�̬����ִ�У����ڳ�ʼ��Ӳ����ʱ��ͨ��ǩ���ļ�����ִ�д������ص����ڣ��������ñ��������ڡ�
*               �� slm_execute_dynamic ���ڶ�̬����ִ�У�����ǰͨ��d2cAPIǩ�ÿ�ִ�д��������Ӧ�ó���һ�𣬵���Ҫִ��ʱ���ص����ڣ�ִ����ɺ����ڻ�ɾ���˳��򣬲������ڳ�ʱ�䱣����
*   @code
*       {
*           SS_BYTE     exf_buff[MAX_BUFFER_SIZE] = { 0 };       // ��̬����������������
*           SS_UINT32   exf_size = 0;                            // ��̬������������С
*           SS_BYTE     inbuff[MAX_BUFFER_SIZE] = { 0 };
*           SS_BYTE     outbuff[MAX_BUFFER_SIZE] = { 0 };
*           SS_UINT32   retlen = 0;
*           SS_UINT32   status = 0;
*
*           // 1������̬���뻺������ֵ��
*           // {...}
*
*           memcpy(inbuff, "1234567890", 10);
*
*           status = slm_execute_static(slm_handle, exf_buff, exf_size, inbuff, 10, outbuff, MAX_BUFFER_SIZE, &retlen);
*           if(status != SS_OK)
*           {
*               //todo do: deal error code
*               return ;
*           }
*           // todo: �������ڳ���ķ�������
*       }
*   @endcode
*   @see        slm_login slm_execute_static slm_snippet_execute
*/
SS_UINT32 SSAPI slm_execute_dynamic(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_BYTE*            exf_buffer,
    IN  SS_UINT32           exf_size,
    IN  SS_BYTE*            inbuf,
    IN  SS_UINT32           insize,
    OUT SS_BYTE*            poutbuf,
    IN  SS_UINT32           outsize,
    OUT SS_UINT32*          pretsize
    );

/*!
*   @brief      Virbox���ɷ��� �ڴ��й��ڴ�����
*   @param[in]  slm_handle   ���ɾ��ֵ����slm_login�õ�
*   @param[in]  size         �����ڴ��С����� SLM_MEM_MAX_SIZE.
*   @param[out] mem_id       �����й��ڴ�id
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    Virbox���ɷ��� ���й��ڴ�ԭ���� APP������Ч��������Ϊƾ֤���� Virbox���ɷ��� ģ�������ݼ���������У�飬���ڴ����������û�����ģ�
*               �����޷��Ƿ��޸ġ��ڿͼ��Ѳ鿴��۸�ʹ�á�
*
*               �û����԰��Լ�APP��һЩ�������ݱ��浽 Virbox���ɷ��� ���й��ڴ棬�����ʺſ��SQL���ݿ���ʺ������룬�漰������Ȩ�޵���ʱ���ݷŵ� Virbox���ɷ��� �ڴ��й����档
*               ����һ����APP�� Virbox���ɷ��� ��϶ȼ������ߣ���ֹ�ڿ����� Virbox���ɷ��� ���������С�
*
*               �ڴ��йܵĺô���
*               -# ���������ڴ治й�ܡ��޷��۸ġ�
*               -# ���Կ��̰߳�ȫ�������ݡ�
*               -# APP���������ɡ�Virbox���ɷ��� ����ǿ��ϣ�������ֹ���ƽ��������ߡ����ڿ���Ҫ�ֹ�������ؽ�����ʹ������
*
*   @code
*       {
*           SS_UINT32 status = SS_OK;
*           SS_UINT32 mem_index = 0;
*           SS_UINT32 mem_size = 1024;
*           SS_BYTE   data[] = "test data....";
*           SS_BYTE   read[100] = { 0 };
*           SS_UINT32 write_len = 0;
*           SS_UINT32 read_len = 0;
*
*           status = slm_mem_alloc(slm_handle, mem_size, &mem_index);
*           if(status != SS_OK)
*           {
*               //todo do: deal error code
*               return ;
*           }
*           status = slm_mem_write(slm_handle, mem_index, 0, strlen((char *)data), data, &write_len);
*           if(status != SS_OK)
*           {
*               //todo do: deal error code
*               return ;
*           }
*           status = slm_mem_read(slm_handle, mem_index, 0, write_len, read, &read_len);
*           if(status != SS_OK)
*           {
*               //todo do: deal error code
*               return ;
*           }
*           //�Ա�ԭʼ���ݺͶ�ȡ���������Ƿ����
*           //if(data == read)
*           //{
*           //    SUCCESS;
*           //}
*           //else
*           //{
*           //    FAILURE;
*           //}

*           status = slm_mem_free(slm_handle, mem_index);
*       }
*   @endcode
*   @see        slm_mem_free slm_mem_read slm_mem_write
*/
SS_UINT32 SSAPI slm_mem_alloc(
    IN  SLM_HANDLE_INDEX    slm_handle, 
    IN  SS_UINT32           size, 
    OUT SS_UINT32*          mem_id
    );
/*!
*   @brief     �ͷ��й��ڴ�
*   @param[in] slm_handle    ���ɾ��ֵ����slm_login�õ�
*   @param[in] mem_id        �й��ڴ�id
*   @return    �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @code
*       //see slm_mem_alloc
*   @endcode
*   @see       slm_mem_alloc slm_mem_free slm_mem_read slm_mem_write
*/
SS_UINT32 SSAPI slm_mem_free(
    IN  SLM_HANDLE_INDEX    slm_handle, 
    IN  SS_UINT32           mem_id
    );

/*!
*   @brief      Virbox���ɷ��� �ڴ��йܶ�
*   @param[in]  slm_handle    ���ɾ��ֵ����slm_login�õ�
*   @param[in]  mem_id        �й��ڴ�id
*   @param[in]  offset        ��ȡ�й�����ƫ��
*   @param[in]  len           ��ȡ�й����ݳ���
*   @param[out] readbuff      �ⲿ�洢�й����ݻ���
*   @param[out] readlen       ����ʵ�ʶ�ȡ����
*   @return    �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @code
*       //see slm_mem_alloc
*   @endcode
*   @see slm_mem_alloc slm_mem_free slm_mem_write
*/
SS_UINT32 SSAPI slm_mem_read(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_UINT32           mem_id,
    IN  SS_UINT32           offset,
    IN  SS_UINT32           len,
    IN  SS_BYTE*            readbuff,
    OUT SS_UINT32*          readlen
    );

/*!
*   @brief      Virbox���ɷ��� �ڴ��й��ڴ�д��
*   @param[in]  slm_handle    ���ɾ��ֵ����slm_login�õ�
*   @param[in]  mem_id        �й��ڴ�id
*   @param[in]  offset        �й�����ƫ��
*   @param[in]  len           д����й����ݳ���
*   @param[in]  writebuff     ��Ҫд���й��ڴ���������ݻ���
*   @param[out] numberofbyteswritten ����ʵ��д�ĳ���
*   @return     �ɹ�����SS_OK��ʧ���򷵻���Ӧ�Ĵ�����
*   @code
*       //see slm_mem_alloc
*   @endcode
*   @see slm_mem_alloc slm_mem_free slm_mem_read
*/
SS_UINT32 SSAPI slm_mem_write(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_UINT32           mem_id,
    IN  SS_UINT32           offset,
    IN  SS_UINT32           len,
    IN  SS_BYTE*            writebuff,
    OUT SS_UINT32*          numberofbyteswritten
    );

/*!
*   @brief      ����Ƿ����ڵ���
*   @param[in]  auth     ��֤����(������������NULL���ɣ�
*   @return     SS_UINT32������, ����SS_OK����δ����
*   @remark     �������slm_init֮��ſ�ʹ�ã������п��ܻ���ɳ������
*/
SS_UINT32 SSAPI slm_is_debug(IN void *auth);

/*!
*   @brief      ͨ��֤�����ͣ���ȡ�ѵ�¼���ɵ��豸֤��
*   @param[in]  slm_handle  ���ɾ��
*   @param[in]  cert_type   ֤�����ͣ��ο� CERT_TYPE_XXX
*   @param[out] cert        ֤�黺����
*   @param[in]  cert_size   ��������С
*   @param[out] cert_len    ���ص��豸֤���С
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ��ȡ�豸֤�飬�˴����豸ָӲ������������
*               ��� cert_type = CERT_TYPE_DEVICE_CERT���书���� slm_get_device_cert ��ȫһ�£�
*               ���Ϊ�������ͣ����֧��Ӳ������
*               ͨ���˽ӿڿ��Ի�ȡ�������ĸ�֤�顢�豸��CA���豸������ϳ��豸֤������
*   @see    slm_login slm_get_device_cert
*/
SS_UINT32 SSAPI slm_get_cert(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  CERT_TYPE           cert_type,
    OUT SS_BYTE*            cert,
    IN  SS_UINT32           cert_size,
    OUT SS_UINT32*          cert_len
    );

/*!
*   @brief      ��ȡ�ѵ�¼�ļ�����֤��
*   @param[in]  slm_handle  ���ɾ������slm_login�õ�
*   @param[out] device_cert �豸֤�黺����
*   @param[in]  buff_size   ��������С
*   @param[out] return_size ���ص��豸֤���С
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ��ȡ�ѵ�¼�ļ�����֤�飬�˴��ļ�����ָӲ����������������
*   @see        slm_login
*/
SS_UINT32 SSAPI slm_get_device_cert(
    IN  SLM_HANDLE_INDEX    slm_handle,
    OUT SS_BYTE*            device_cert,
    IN  SS_UINT32           buff_size,
    OUT SS_UINT32*          return_size
    );

/*!
*   @brief      �豸������֤����֧��Ӳ������
*   @param[in]  slm_handle          ���ɾ������slm_login�õ�
*   @param[in]  verify_data         ��֤���ݣ��������ַ�"SENSELOCK"(9�ֽ�)��ͷ��
*   @param[in]  verify_data_size    ��֤���ݴ�С����С����Ϊ SLM_VERIFY_DATA_SIZE(41)���ֽ�
*   @param[out] signature           ���ص�ǩ�����
*   @param[in]  signature_buf_size  ��������С
*   @param[out] signature_size      ǩ�������С
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @remarks    ����ʹ���豸˽Կǩ������Ҫ��Ӳ����֤�鹫Կ��ǩ
*   @code
*       {
*           SS_UINT32       status = SS_OK;
*           SS_BYTE         verify_data[41] = {0};
*           SS_BYTE         my_data[32] = {"verify data..."};
*           const SS_BYTE   header[9] = {"SENSELOCK"};
*           SS_BYTE         signature[2048] = {0};
*           SS_UINT32       signature_size = 0;
*       
*           memcpy(verify_data, header, sizeof(header));
*           memcpy(verify_data + sizeof(header), my_data, sizeof(my_data));
*           
*           status = slm_sign_by_device(slm_handle, verify_data, sizeof(verify_data), signature, sizeof(signature), &signature_size);
*           if(status != SS_OK)
*           {
*               // todo: deal error code
*           }
*       }
*   @endcode
*   @see        slm_login
*/
SS_UINT32 SSAPI slm_sign_by_device(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_BYTE*            verify_data,
    IN  SS_UINT32           verify_data_size,
    OUT SS_BYTE*            signature,
    IN  SS_UINT32           signature_buf_size,
    OUT SS_UINT32*          signature_size
    );

/*!
*   @brief         ��ȡʱ���޸����ݣ���������ʱ��У׼����
*   @param[in]     slm_handle  ���ɾ������slm_login�õ�
*   @param[out]    rand        �����
*   @param[out]    lock_time   ��ʱ��
*   @param[in,out] pc_time     PCʱ�䣬���ص�PCʱ�䣨����0��ȡ��ʱʱ�䣩
*   @return        �ɹ�����SS_OK��ʧ���򷵻���Ӧ������
*/
SS_UINT32 SSAPI slm_adjust_time_request(
    IN  SLM_HANDLE_INDEX    slm_handle,
    OUT SS_BYTE             rand[SLM_FIXTIME_RAND_LENGTH],
    OUT SS_UINT32*          lock_time,
    IN OUT SS_UINT32*       pc_time
    );

/*!
*   @brief      ��˸ָʾ��
*   @param[in]  slm_handle      ���ɾ������slm_login�õ�
*   @param[in]  led_ctrl        ���ƿ��ƽṹ(ST_LED_CONTROL)
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @code
*       {
*           ST_LED_CONTROL led = { 0 };
*           SS_UINT32 status = SS_OK;
*    
*           led.index = 0;         //  0��ʾ��ɫLED��1��ʾ��ɫLED  
*           led.state = 2;         //  0�����رգ�1�����򿪣� 2������˸ 
*           led.interval = 1000;   //  ��˸��������룩
*
*           status = slm_led_control(slm_handle, &led);
*           if(status != SS_OK)
*           {
*               // todo: deal error code
*           }
*       }
*   @endcode
*   @see        slm_login ST_LED_CONTROL
*/
SS_UINT32 SSAPI slm_led_control(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  ST_LED_CONTROL*     led_ctrl
    );

/*!
*   @brief      ��� Runtime API �� Virbox���ɷ��� �İ汾��Ϣ.
*   @param[out] api_version  API�İ汾�����ǳɹ���
*   @param[out] ss_version   Virbox���ɷ��� �İ汾
*   @return     �ɹ�����SS_OK��ʧ���򷵻���Ӧ�Ĵ�����
*/
SS_UINT32 SSAPI slm_get_version(
    OUT SS_UINT32*      api_version, 
    OUT SS_UINT32*      ss_version
    );

/*!
*   @brief     �ͷ�API�ڷ��������
*   @param[in] buffer API���ɵĶ�
*/
void SSAPI slm_free(
    IN void*        buffer
    );

/*!
*   @brief      ��D2C������������D2C����d2cAPI����
*   @param[in]  d2c_pkg     d2c�ļ�����
*   @param[out] error_msg   ������Ϣ����Ҫ����slm_free�ͷ�
*   @return     �ɹ�����SS_OK��ʧ�ܷ��ش�����
*   @remarks:   D2C���ڿ��ܰ����������ݣ���˷��صĴ�������ϢҲ��json����ṹ����Ҫ�ֱ����
*   @code
*     [
*       {"pkg_order":1, "pkg_desc":"package decription.", "status": 0},
*       {"pkg_order":2, "pkg_desc":"package decription.", "status": 0}
*     ]
*   @endcode
*/
SS_UINT32 SSAPI slm_update(
    IN  char*       d2c_pkg, 
    OUT char**      error_msg
    );

/*!
*   @brief      ��D2C��������������ָ��������Ψһ���кţ�D2C����d2cAPI����
*   @param[in]  lock_sn     ���ţ�Ψһ���к�,ʮ�������ַ�����
*   @param[in]  d2c_pkg     d2c�ļ�����
*   @param[out] error_msg   ������Ϣ����ʹ����Ҫ����slm_free�ͷ�
*   @return     �ɹ�����SS_OK��ʧ�ܷ��ش�����
*   @remarks:  D2C���ڿ��ܰ����������ݣ���˷��صĴ�������ϢҲ��json����ṹ����Ҫ�ֱ����
*   @code
*     [
*       {"pkg_order":1, "pkg_desc":"package decription.", "status": 0},
*       {"pkg_order":2, "pkg_desc":"package decription.", "status": 0}
*     ]
*   @endcode
*   @see    slm_update
*/
SS_UINT32 SSAPI slm_update_ex(
    IN SS_BYTE*     lock_sn, 
    IN char*        d2c_pkg, 
    OUT char**      error_msg
    );

/*!
*   @brief      ���ڶ�����������ָ��������Ψһ���кţ������ڿ�ִ���㷨��ϣ���֧��Ӳ������
*   @param[in]  lock_sn     ���ţ�Ψһ���к�,ʮ�������ַ�����
*   @param[in]  inside_file ���ڶ����ļ���
*   @return     �ɹ�����SS_OK��ʧ�ܷ��ش�����
*   @remarks    Virbox �ṩ��ͨ�����뼤�����Ӳ����������һ�� Virbox���ɣ��ù�����Ҫͨ����ִ���㷨����ϣ��򵥵Ĳ�������Ϊ��
*               ��һ��������������֧�ֶ��뼤���ʹ���ļ�slac_enable.evd���ļ��������⣬��С��ò�����256�ֽڣ�
*               �ڶ�������д���ڿ�ִ���㷨��������˼�ṩ��h5ses.lib��h5ses_lm.lib�⣬ʵ�ֶ���ת���Ĺ��̣�
*               ��������ͨ��ִ�����ڿ�ִ���㷨�������� slm_execute_static �� ������������ݣ���ʹ���ڿ�ִ���㷨�����ڽ�����ת��Ϊ
*                       Ӳ������ʶ����м��ļ����ļ����������趨����
*               ���Ĳ���ͨ������slm_d2c_update_inside �ӿڣ����������ɵ��м��ļ�ת��Ϊ��ʽ�� Virbox ���ɡ�
*
*               �ر�˵�������뼤���ʹ�ý�Ϊ���ӣ���Ҫд���ڴ���ļ���Ҫ������������Ҫʹ�ô˹��ܣ�����ϵ��˼�����������֧�֡�
*   @code
*       {
*           SS_UINT32 status = SS_OK;
*           SS_BYTE   data_buf[1024] = { 0 };
*           SS_UINT32 data_size = 0;
*           char *sn = "9733c80100070205106100030015000c";
*
*           memcpy(data_buf, "1234567890", 10);
*
*           status = slm_execute_static(slm_handle, "test.evx", NULL, 0, data_buf, 10, &data_size);
*           if(status != SS_OK)
*           {
*               // todo: deal error code
*           }
*
*           status = slm_d2c_update_inside(sn, "test.evd");
*           if(status != SS_OK)
*           {
*               // todo: deal error code
*           }
*       }
*   @endcode
*   @see    slm_execute_static
*/
SS_UINT32 SSAPI slm_d2c_update_inside(
    IN char*        lock_sn,
    IN char*        inside_file
    );

/*!
*   @brief       ö�ٱ�������Ϣ
*   @param[out]  device_info    �豸������Ϣ������ʹ��ʱ�������slm_free�ͷ�
*   @return      �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks     �˽ӿڿ���ö�ٵ����������豸��Ϣ���˴����豸��Ϣ���ݿɲο� slm_get_info LOCK_INFO�еĽṹ����
*   @code
*       // �ο� slm_get_license_info ��ʾ������
*   @endcode
*   @see slm_enum_device slm_enum_license_id slm_get_license_info
*/
SS_UINT32 SSAPI slm_enum_device(
    OUT char**  device_info
    );

/*!
*   @brief       ö�ٱ�������Ϣ��֧�ְ�����ֵ���ͻ�ȡ
*   @param[in]   format         ��Ϣ��ʽ���ο� INFO_FORMAT_TYPE����ǰ��֧��JSON��STRUCT
*   @param[out]  device_info    �豸������Ϣ������ʹ��ʱ�������slm_free�ͷ�
*   @return      �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks     �˽ӿڿ���ö�ٵ����������豸��Ϣ���˴����豸��Ϣ���ݿɲο� slm_get_info LOCK_INFO�еĽṹ����
*   @code
*       // format = JSON
*           device_info ���ݺ� slm_enum_device ��������һ�£� �ο� slm_get_license_info ��ʾ������
*       // format = STRUCT
            device_info ���ݲο� ST_DEV_INFO_LIST���ο� slm_get_license_info_ex ʾ������
*   @endcode
*   @see slm_enum_device slm_enum_license_id slm_get_license_info
*/
SS_UINT32 SSAPI slm_enum_device_ex(
    IN  INFO_FORMAT_TYPE    format,
    OUT void                **device_info
    );

/*!
*   @brief       ö��ָ���豸����������ID
*   @param[in]   device_info   ָ��ĳ������½
*   @param[out]  license_ids   �����������ɵ�ID���飬JSON��ʽ����Ҫ����slm_free �ͷ� license_ids
*   @return      �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks     ��slm_enum_device��ȡ����ǰ�豸��Ϣ��ͨ���豸��Ϣ��ȡ������Ϣ����Ҫʵ�ֲ��õ�¼���ɣ���ɲ鿴�������ݵĹ���
*   @code
*       // �ο� slm_get_license_info ʾ������
*   @endcode
*   @see slm_enum_device slm_enum_license_id slm_get_license_info
*/
SS_UINT32 SSAPI slm_enum_license_id(
    IN const char   *device_info,
    OUT char        **license_ids
    );

/*!
*   @brief       ö��ָ���豸����������ID��֧�ְ�����ֵ���ͻ�ȡ
*   @param[in]   device_info    ָ��ĳ������Ϣ
*   @param[in]   format         ��Ϣ��ʽ���ο� INFO_FORMAT_TYPE����ǰ��֧��JSON��STRUCT
*   @param[out]  license_ids    �����������ɵ�ID���飬JSON��ʽ����Ҫ����slm_free �ͷ� license_ids
*   @return      �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks     ��slm_enum_device��ȡ����ǰ�豸��Ϣ��ͨ���豸��Ϣ��ȡ������Ϣ����Ҫʵ�ֲ��õ�¼���ɣ���ɲ鿴�������ݵĹ���
*   @code
*       //  �ο� slm_get_license_info_ex ʾ������
*   @endcode
*   @see slm_enum_device slm_enum_license_id slm_get_license_info
*/
SS_UINT32 SSAPI slm_enum_license_id_ex(
    IN  ST_DEV_INFO         *device_info,
    IN  INFO_FORMAT_TYPE    format,
    OUT void                **license_ids
    );

/*!
*   @brief       ��ȡָ���豸��ָ�����ɵ�ȫ����Ϣ
*   @param[in]   device_info    ָ��ĳ������½
*   @param[in]   license_id     ָ������ID
*   @param[out]  license_info   �������ɵ���ϢJSON��ʽ ��ͬ��slm_get_info��LICENSE_INFO
*   @return      �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks     ��ȡ��ָ���豸������ID�б�������ͳ��������������
*   @code
*       {
*           char        *lic_id = NULL;
*           char        *dev_desc = NULL;
*           char        *lic_info = NULL;
*           SS_UINT32   status = SS_OK;
*           Json::Reader reader;      // �˴�ѡ��jsoncpp����json����
*           Json::Value  root;
*           Json::Value  lic;
*
*           status = slm_enum_device(&dev_desc);    // ����Ҫ�ȱ��������豸
*           if ( status == SS_OK && dev_desc != NULL && reader.parse(dev_desc, root))
*           {
*               for (int i = 0; i < root.size(); i++)
*               {
*                   status = slm_enum_license_id(root[i].toStyledString().c_str(), &lic_id);    // ��λ�ȡÿ���豸������ID
*                   if (status == SS_OK && lic_id != NULL)
*                   {
*                       printf(lic_id);
*                       printf("\n");
*
*                       if (reader.parse(lic_id, lic))
*                       {
*                           for (int j = 0; j < lic.size(); j++)
*                           {
*                               status = slm_get_license_info(root[i].toStyledString().c_str(), lic[j].asInt(), &lic_info);  // ����ȡ���ɵ���ϸ��Ϣ
*                               if (lic_info)
*                               {
*                                   printf(lic_info);
*                                   printf("\n");
*                                   slm_free(lic_info);
*                                   lic_info = NULL;
*                               }
*                           }
*                       }
*                       slm_free(lic_id);
*                       lic_id = NULL;
*                   }
*               }
*               slm_free(dev_desc);
*               dev_desc = NULL;
*           }
*       }
*   @endcode
*   @see slm_enum_device slm_enum_license_id slm_get_license_info
*/
SS_UINT32 SSAPI slm_get_license_info(
    IN const char   *device_info,
    IN SS_UINT32    license_id,
    OUT char        **license_info
    );

/*!
*   @brief       ��ȡָ���豸��ָ�����ɵ�ȫ����Ϣ��֧�ְ�����ֵ���ͻ�ȡ
*   @param[in]   device_info    ָ��ĳ������½
*   @param[in]   format         ��Ϣ��ʽ���ο� INFO_FORMAT_TYPE����ǰ��֧��JSON��STRUCT
*   @param[in]   license_id     ָ������ID
*   @param[out]  license_info   �������ɵ���ϢJSON��ʽ ��ͬ��slm_get_info��LICENSE_INFO
*   @return      �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks     ��ȡ��ָ���豸������ID�б�������ͳ��������������
*   @code
*       {
*           void        *dev_desc = NULL;
*           SS_UINT32   dev_count = 0;
*           void        *lic_desc = NULL;
*           void         *lic_info = NULL;
*           int         status = SS_OK;
*           
*           status = slm_enum_device_ex(STRUCT, &dev_desc);
*           
*           if (status == SS_OK && dev_desc != NULL)
*           {
*               ST_DEV_INFO_LIST *dl = (ST_DEV_INFO_LIST*)dev_desc;
*               //printf("DEV LIST COUNT:%d\n", dl->count);
*               for (int i = 0; i != dl->count; ++i)
*               {
*                   status = slm_enum_license_id_ex(&dl->info[i].dev_info, STRUCT, &lic_desc);
*                   if (status == SS_OK && lic_desc != NULL)
*                   {
*                       ST_LICENSE_IDS  *lic = (ST_LICENSE_IDS*)lic_desc;
*                       //printf("License Count : %d\n", lic->count);
*                       for (int j = 0; j != lic->count; ++j)
*                       {
*                           status = slm_get_license_info_ex(&dl->info[i].dev_info, lic->lic_ids[j], STRUCT, &lic_info);
*                           if (status == SS_OK && lic_info != NULL)
*                           {
*                               //printf("LicenseID = %d; first use time = %d\n", ((ST_SLM_LICENSE_INFO*)lic_info)->lic_id, ((ST_SLM_LICENSE_INFO*)lic_info)->first_use_time);
*                               slm_free(lic_info);
*                               lic_info = NULL;
*                           }
*                       }
*                       slm_free(lic_desc);
*                       lic_desc = NULL;
*                   }
*               }
*               slm_free(dev_desc);
*               dev_desc = NULL;
*           }
*       }
*   @endcode
*   @see slm_enum_device slm_enum_license_id slm_get_license_info
*/
SS_UINT32 SSAPI slm_get_license_info_ex(
    IN  ST_DEV_INFO          *device_info,
    IN  SS_UINT32           license_id,
    IN  INFO_FORMAT_TYPE    format,
    OUT void                **license_info
    );

/*!
*   @brief     ���ģ��
*   @param[in] slm_handle     ���ɾ������slm_login�õ�
*   @param[in] module_id      ģ��ID����Χ�ɣ�1 ~ 64��
*   @return    ģ����ڷ���SS_OK�������ڷ���SS_ERROR_LICENSE_MODULE_NOT_EXISTS, ���򷵻�����������
*/
SS_UINT32 SSAPI slm_check_module(IN SLM_HANDLE_INDEX slm_handle, IN SS_UINT32 module_id);

/*!
*   @brief      ��Ƭ����ִ�У������߲��ع��ģ�
*   @param[in]  slm_handle      ���ɾ��
*   @param[in]  snippet_code    ��Ƭ����
*   @param[in]  code_size       ��Ƭ�����С
*   @param[in]  input           ��������
*   @param[in]  input_size      �������ݳ���
*   @param[out] output          ���������
*   @param[in]  outbuf_size     �������������
*   @param[out] output_size     ������ݳ���
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ�Ĵ�����
*   @see        slm_login slm_execute_static slm_execute_dynamic
*/
SS_UINT32 SSAPI slm_snippet_execute(
    IN  SLM_HANDLE_INDEX    slm_handle,
    IN  SS_BYTE*            snippet_code,
    IN  SS_UINT32           code_size,
    IN  SS_BYTE*            input, 
    IN  SS_UINT32           input_size, 
    OUT SS_BYTE*            output, 
    IN  SS_UINT32           outbuf_size, 
    OUT SS_UINT32*          output_size
    );

/*!
*   @brief      ��ȡAPI��Ӧ�Ŀ�����ID
*   @param[out] developer_id          ���������ID������������
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks    ���developer_id�Ļ�����С��SLM_DEVELOPER_ID_SIZE�����Ʊػ������ջ�����
*/
SS_UINT32 SSAPI slm_get_developer_id(OUT SS_BYTE developer_id[SLM_DEVELOPER_ID_SIZE]);

/*!
*  @brief      ʹ���ѵ�¼�������ɽ���ǩ������֧��������
*  @param[in]  slm_handle       ���ɾ������slm_login�õ�
*  @param[in]  sign_data        Ҫǩ�������ݣ�����16�ֽڣ����64�ֽڣ�
*  @param[in]  sign_length      Ҫǩ�������ݳ���
*  @param[out] signature        ǩ�����������
*  @param[in]  max_buf_size     ǩ�������������С
*  @param[out] signature_length ǩ�����ʵ�ʳ���
*  @return     �ɹ�����SS_OK��ʧ�ܷ��ش�����
*  @remarks    ��ͬ�����ߡ���ͬ���ɵ�ǩ������ǲ�һ���ġ�
*  @see        slm_license_verify 
*/
SS_UINT32 SSAPI slm_license_sign(
    IN  SLM_HANDLE_INDEX   slm_handle,
    IN  SS_BYTE           *sign_data,
    IN  SS_UINT32          sign_length,
    OUT SS_BYTE           *signature,
    IN  SS_UINT32          max_buf_size,
    OUT SS_UINT32         *signature_length
    );

/*!
*  @brief      ��������ǩ��������ݽ�����ǩ����֧��������
*  @param[in]  sign_data        Ҫǩ�������ݣ�����16�ֽڣ����64�ֽڣ�
*  @param[in]  sign_length      Ҫǩ�������ݳ���
*  @param[in]  signature        ǩ���������
*  @param[in]  signature_length ǩ���������
*  @param[out] sign_info        ǩ��������Ϣ��json�ṹ������ʹ��ʱ����Ҫ����slm_free�ͷ�
*  @return     ��ǩ�ɹ�����SS_OK��ʧ�ܷ��ش�����
*  @remarks   -����ɹ�����Ҫ����slm_free �ͷ�sign_info
*             -��ǩ���̿��Բ���¼����
*  @code 
   //json����
   {
       "type":2,                                   // 2��ʾ����
       "developer_id":0000000000000000,            // ������ID
       "license_id":0,                             // ����ID
       "guid":xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,    // �û�guid�������;�����
       "sn":00000000000000000000000000000000,      // ���ţ������;�����
       "rand":0,                                   // �����
   }
*  @see  slm_license_sign slm_free
*/
SS_UINT32 SSAPI slm_license_verify(
    IN  SS_BYTE      *sign_data,
    IN  SS_UINT32       sign_length,
    IN  SS_BYTE      *signature,
    IN  SS_UINT32     signature_length,
    OUT char        **sign_info
    );

/*!
*   @brief      ͨ���������ô�����Ϣ
*   @param[in]  error_code  ͨ����API����ʧ�ܺ󷵻صĴ�����ֵ
*   @param[in]  language_id Ҫ�����ַ��������ԣ��� LANGUAGE_XXXX_ASCII
*   @return     �ɹ����ش������ı�������Ϣ������Ҫ����slm_free�ͷţ���ʧ�ܷ��ؿ�ָ��NULL 
*/
const SS_CHAR * SSAPI slm_error_format(
    IN SS_UINT32    error_code,
    IN SS_UINT32    language_id
    );

/*!
*   @brief      ����ʼ����������slm_init��Ӧ
*   @see        slm_init
*   @remarks    slm_cleanup�Ƿ��̰߳�ȫ�ģ��˺��������鿪���ߵ��ã���Ϊ�����˳�ʱϵͳ���Զ�����û���ͷŵ��ڴ棬
*               �������ߵ��ã�Ϊ�˱�֤���̵߳���slm_runtime_api�İ�ȫ�ԣ��˺��������ڳ����˳�ʱ���á�
*               һ�������˴˺�������������API����slm_init��������ʹ�á�
*/
SS_UINT32 SSAPI slm_cleanup(void);

/*!
*   @brief      ��չ���ýӿ�
*   @param[in]  config    ���ò�����JSON��
*               1.slm_login ��չ������ָ�����ɵ�¼���������������ַ��ע������ַURL���Ȳ��ó��� SLM_MAX_CLOUD_SERVER_LENGTH �궨�峤�ȡ�
*                           {"api_slm_login":{"cloud_runtime_url":"http://rt.senseyun.com", "slock_url":"http://sl.senseyun.com"}}
*   @return     �ɹ�����SS_OK��ʧ�ܷ�����Ӧ������
*   @remarks    ��֧�ֶ��̵߳��ã����̰߳�ȫ���뱣֤��������ع�������֮ǰִ�С�
*/
SS_UINT32 SSAPI slm_extensions_config(IN const char *config);

#ifdef __cplusplus
};
#endif //__cplusplus

/**
*   @}
*/


#endif // #ifndef __SS_LM_RUMTIME_H__
