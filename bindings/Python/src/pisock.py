# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _pisock

def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


error = _pisock.error 


print_splash = _pisock.print_splash

pilot_connect = _pisock.pilot_connect
PI_AF_PILOT = _pisock.PI_AF_PILOT
PI_SOCK_STREAM = _pisock.PI_SOCK_STREAM
PI_SOCK_RAW = _pisock.PI_SOCK_RAW
PI_CMD_CMP = _pisock.PI_CMD_CMP
PI_CMD_NET = _pisock.PI_CMD_NET
PI_CMD_SYS = _pisock.PI_CMD_SYS
PI_MSG_PEEK = _pisock.PI_MSG_PEEK
PI_MSG_REALLOC = _pisock.PI_MSG_REALLOC
PI_PF_DEV = _pisock.PI_PF_DEV
PI_PF_SLP = _pisock.PI_PF_SLP
PI_PF_SYS = _pisock.PI_PF_SYS
PI_PF_PADP = _pisock.PI_PF_PADP
PI_PF_NET = _pisock.PI_PF_NET
PI_PF_DLP = _pisock.PI_PF_DLP
PI_LEVEL_DEV = _pisock.PI_LEVEL_DEV
PI_LEVEL_SLP = _pisock.PI_LEVEL_SLP
PI_LEVEL_PADP = _pisock.PI_LEVEL_PADP
PI_LEVEL_NET = _pisock.PI_LEVEL_NET
PI_LEVEL_SYS = _pisock.PI_LEVEL_SYS
PI_LEVEL_CMP = _pisock.PI_LEVEL_CMP
PI_LEVEL_DLP = _pisock.PI_LEVEL_DLP
PI_LEVEL_SOCK = _pisock.PI_LEVEL_SOCK
PI_DEV_RATE = _pisock.PI_DEV_RATE
PI_DEV_ESTRATE = _pisock.PI_DEV_ESTRATE
PI_DEV_HIGHRATE = _pisock.PI_DEV_HIGHRATE
PI_DEV_TIMEOUT = _pisock.PI_DEV_TIMEOUT
PI_SLP_DEST = _pisock.PI_SLP_DEST
PI_SLP_LASTDEST = _pisock.PI_SLP_LASTDEST
PI_SLP_SRC = _pisock.PI_SLP_SRC
PI_SLP_LASTSRC = _pisock.PI_SLP_LASTSRC
PI_SLP_TYPE = _pisock.PI_SLP_TYPE
PI_SLP_LASTTYPE = _pisock.PI_SLP_LASTTYPE
PI_SLP_TXID = _pisock.PI_SLP_TXID
PI_SLP_LASTTXID = _pisock.PI_SLP_LASTTXID
PI_PADP_TYPE = _pisock.PI_PADP_TYPE
PI_PADP_LASTTYPE = _pisock.PI_PADP_LASTTYPE
PI_CMP_TYPE = _pisock.PI_CMP_TYPE
PI_CMP_FLAGS = _pisock.PI_CMP_FLAGS
PI_CMP_VERS = _pisock.PI_CMP_VERS
PI_CMP_BAUD = _pisock.PI_CMP_BAUD
PI_NET_TYPE = _pisock.PI_NET_TYPE
PI_NET_SPLIT_WRITES = _pisock.PI_NET_SPLIT_WRITES
PI_NET_WRITE_CHUNKSIZE = _pisock.PI_NET_WRITE_CHUNKSIZE
PI_SOCK_STATE = _pisock.PI_SOCK_STATE
class pi_socket_t(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, pi_socket_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, pi_socket_t, name)
    def __repr__(self):
        return "<C pi_socket_t instance at %s>" % (self.this,)
    __swig_setmethods__["sd"] = _pisock.pi_socket_t_sd_set
    __swig_getmethods__["sd"] = _pisock.pi_socket_t_sd_get
    if _newclass:sd = property(_pisock.pi_socket_t_sd_get, _pisock.pi_socket_t_sd_set)
    __swig_setmethods__["type"] = _pisock.pi_socket_t_type_set
    __swig_getmethods__["type"] = _pisock.pi_socket_t_type_get
    if _newclass:type = property(_pisock.pi_socket_t_type_get, _pisock.pi_socket_t_type_set)
    __swig_setmethods__["protocol"] = _pisock.pi_socket_t_protocol_set
    __swig_getmethods__["protocol"] = _pisock.pi_socket_t_protocol_get
    if _newclass:protocol = property(_pisock.pi_socket_t_protocol_get, _pisock.pi_socket_t_protocol_set)
    __swig_setmethods__["cmd"] = _pisock.pi_socket_t_cmd_set
    __swig_getmethods__["cmd"] = _pisock.pi_socket_t_cmd_get
    if _newclass:cmd = property(_pisock.pi_socket_t_cmd_get, _pisock.pi_socket_t_cmd_set)
    __swig_setmethods__["laddr"] = _pisock.pi_socket_t_laddr_set
    __swig_getmethods__["laddr"] = _pisock.pi_socket_t_laddr_get
    if _newclass:laddr = property(_pisock.pi_socket_t_laddr_get, _pisock.pi_socket_t_laddr_set)
    __swig_setmethods__["laddrlen"] = _pisock.pi_socket_t_laddrlen_set
    __swig_getmethods__["laddrlen"] = _pisock.pi_socket_t_laddrlen_get
    if _newclass:laddrlen = property(_pisock.pi_socket_t_laddrlen_get, _pisock.pi_socket_t_laddrlen_set)
    __swig_setmethods__["raddr"] = _pisock.pi_socket_t_raddr_set
    __swig_getmethods__["raddr"] = _pisock.pi_socket_t_raddr_get
    if _newclass:raddr = property(_pisock.pi_socket_t_raddr_get, _pisock.pi_socket_t_raddr_set)
    __swig_setmethods__["raddrlen"] = _pisock.pi_socket_t_raddrlen_set
    __swig_getmethods__["raddrlen"] = _pisock.pi_socket_t_raddrlen_get
    if _newclass:raddrlen = property(_pisock.pi_socket_t_raddrlen_get, _pisock.pi_socket_t_raddrlen_set)
    __swig_setmethods__["protocol_queue"] = _pisock.pi_socket_t_protocol_queue_set
    __swig_getmethods__["protocol_queue"] = _pisock.pi_socket_t_protocol_queue_get
    if _newclass:protocol_queue = property(_pisock.pi_socket_t_protocol_queue_get, _pisock.pi_socket_t_protocol_queue_set)
    __swig_setmethods__["queue_len"] = _pisock.pi_socket_t_queue_len_set
    __swig_getmethods__["queue_len"] = _pisock.pi_socket_t_queue_len_get
    if _newclass:queue_len = property(_pisock.pi_socket_t_queue_len_get, _pisock.pi_socket_t_queue_len_set)
    __swig_setmethods__["cmd_queue"] = _pisock.pi_socket_t_cmd_queue_set
    __swig_getmethods__["cmd_queue"] = _pisock.pi_socket_t_cmd_queue_get
    if _newclass:cmd_queue = property(_pisock.pi_socket_t_cmd_queue_get, _pisock.pi_socket_t_cmd_queue_set)
    __swig_setmethods__["cmd_len"] = _pisock.pi_socket_t_cmd_len_set
    __swig_getmethods__["cmd_len"] = _pisock.pi_socket_t_cmd_len_get
    if _newclass:cmd_len = property(_pisock.pi_socket_t_cmd_len_get, _pisock.pi_socket_t_cmd_len_set)
    __swig_setmethods__["device"] = _pisock.pi_socket_t_device_set
    __swig_getmethods__["device"] = _pisock.pi_socket_t_device_get
    if _newclass:device = property(_pisock.pi_socket_t_device_get, _pisock.pi_socket_t_device_set)
    __swig_setmethods__["state"] = _pisock.pi_socket_t_state_set
    __swig_getmethods__["state"] = _pisock.pi_socket_t_state_get
    if _newclass:state = property(_pisock.pi_socket_t_state_get, _pisock.pi_socket_t_state_set)
    __swig_setmethods__["command"] = _pisock.pi_socket_t_command_set
    __swig_getmethods__["command"] = _pisock.pi_socket_t_command_get
    if _newclass:command = property(_pisock.pi_socket_t_command_get, _pisock.pi_socket_t_command_set)
    __swig_setmethods__["accept_to"] = _pisock.pi_socket_t_accept_to_set
    __swig_getmethods__["accept_to"] = _pisock.pi_socket_t_accept_to_get
    if _newclass:accept_to = property(_pisock.pi_socket_t_accept_to_get, _pisock.pi_socket_t_accept_to_set)
    __swig_setmethods__["dlprecord"] = _pisock.pi_socket_t_dlprecord_set
    __swig_getmethods__["dlprecord"] = _pisock.pi_socket_t_dlprecord_get
    if _newclass:dlprecord = property(_pisock.pi_socket_t_dlprecord_get, _pisock.pi_socket_t_dlprecord_set)
    __swig_setmethods__["dlpversion"] = _pisock.pi_socket_t_dlpversion_set
    __swig_getmethods__["dlpversion"] = _pisock.pi_socket_t_dlpversion_get
    if _newclass:dlpversion = property(_pisock.pi_socket_t_dlpversion_get, _pisock.pi_socket_t_dlpversion_set)
    __swig_setmethods__["maxrecsize"] = _pisock.pi_socket_t_maxrecsize_set
    __swig_getmethods__["maxrecsize"] = _pisock.pi_socket_t_maxrecsize_get
    if _newclass:maxrecsize = property(_pisock.pi_socket_t_maxrecsize_get, _pisock.pi_socket_t_maxrecsize_set)
    __swig_setmethods__["last_error"] = _pisock.pi_socket_t_last_error_set
    __swig_getmethods__["last_error"] = _pisock.pi_socket_t_last_error_get
    if _newclass:last_error = property(_pisock.pi_socket_t_last_error_get, _pisock.pi_socket_t_last_error_set)
    __swig_setmethods__["palmos_error"] = _pisock.pi_socket_t_palmos_error_set
    __swig_getmethods__["palmos_error"] = _pisock.pi_socket_t_palmos_error_get
    if _newclass:palmos_error = property(_pisock.pi_socket_t_palmos_error_get, _pisock.pi_socket_t_palmos_error_set)
    def __init__(self, *args):
        _swig_setattr(self, pi_socket_t, 'this', _pisock.new_pi_socket_t(*args))
        _swig_setattr(self, pi_socket_t, 'thisown', 1)
    def __del__(self, destroy=_pisock.delete_pi_socket_t):
        try:
            if self.thisown: destroy(self)
        except: pass

class pi_socket_tPtr(pi_socket_t):
    def __init__(self, this):
        _swig_setattr(self, pi_socket_t, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, pi_socket_t, 'thisown', 0)
        _swig_setattr(self, pi_socket_t,self.__class__,pi_socket_t)
_pisock.pi_socket_t_swigregister(pi_socket_tPtr)

class pi_socket_list_t(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, pi_socket_list_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, pi_socket_list_t, name)
    def __repr__(self):
        return "<C pi_socket_list_t instance at %s>" % (self.this,)
    __swig_setmethods__["ps"] = _pisock.pi_socket_list_t_ps_set
    __swig_getmethods__["ps"] = _pisock.pi_socket_list_t_ps_get
    if _newclass:ps = property(_pisock.pi_socket_list_t_ps_get, _pisock.pi_socket_list_t_ps_set)
    __swig_setmethods__["next"] = _pisock.pi_socket_list_t_next_set
    __swig_getmethods__["next"] = _pisock.pi_socket_list_t_next_get
    if _newclass:next = property(_pisock.pi_socket_list_t_next_get, _pisock.pi_socket_list_t_next_set)
    def __init__(self, *args):
        _swig_setattr(self, pi_socket_list_t, 'this', _pisock.new_pi_socket_list_t(*args))
        _swig_setattr(self, pi_socket_list_t, 'thisown', 1)
    def __del__(self, destroy=_pisock.delete_pi_socket_list_t):
        try:
            if self.thisown: destroy(self)
        except: pass

class pi_socket_list_tPtr(pi_socket_list_t):
    def __init__(self, this):
        _swig_setattr(self, pi_socket_list_t, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, pi_socket_list_t, 'thisown', 0)
        _swig_setattr(self, pi_socket_list_t,self.__class__,pi_socket_list_t)
_pisock.pi_socket_list_t_swigregister(pi_socket_list_tPtr)


pi_socket = _pisock.pi_socket

pi_socket_setsd = _pisock.pi_socket_setsd

pi_getsockname = _pisock.pi_getsockname

pi_getsockpeer = _pisock.pi_getsockpeer

pi_getsockopt = _pisock.pi_getsockopt

pi_setsockopt = _pisock.pi_setsockopt

pi_protocol = _pisock.pi_protocol

pi_protocol_next = _pisock.pi_protocol_next

pi_socket_connected = _pisock.pi_socket_connected

pi_connect = _pisock.pi_connect

pi_bind = _pisock.pi_bind

pi_listen = _pisock.pi_listen

pi_accept = _pisock.pi_accept

pi_accept_to = _pisock.pi_accept_to

pi_close = _pisock.pi_close

pi_send = _pisock.pi_send

pi_recv = _pisock.pi_recv

pi_read = _pisock.pi_read

pi_write = _pisock.pi_write

pi_flush = _pisock.pi_flush

pi_error = _pisock.pi_error

pi_set_error = _pisock.pi_set_error

pi_palmos_error = _pisock.pi_palmos_error

pi_set_palmos_error = _pisock.pi_set_palmos_error

pi_reset_errors = _pisock.pi_reset_errors

pi_version = _pisock.pi_version

pi_maxrecsize = _pisock.pi_maxrecsize

pi_tickle = _pisock.pi_tickle

pi_watchdog = _pisock.pi_watchdog

dlp_strerror = _pisock.dlp_strerror
dlpDBFlagResource = _pisock.dlpDBFlagResource
dlpDBFlagReadOnly = _pisock.dlpDBFlagReadOnly
dlpDBFlagAppInfoDirty = _pisock.dlpDBFlagAppInfoDirty
dlpDBFlagBackup = _pisock.dlpDBFlagBackup
dlpDBFlagHidden = _pisock.dlpDBFlagHidden
dlpDBFlagLaunchable = _pisock.dlpDBFlagLaunchable
dlpDBFlagRecyclable = _pisock.dlpDBFlagRecyclable
dlpDBFlagBundle = _pisock.dlpDBFlagBundle
dlpDBFlagOpen = _pisock.dlpDBFlagOpen
dlpDBFlagNewer = _pisock.dlpDBFlagNewer
dlpDBFlagReset = _pisock.dlpDBFlagReset
dlpDBFlagCopyPrevention = _pisock.dlpDBFlagCopyPrevention
dlpDBFlagStream = _pisock.dlpDBFlagStream
dlpDBFlagSchema = _pisock.dlpDBFlagSchema
dlpDBFlagSecure = _pisock.dlpDBFlagSecure
dlpDBFlagExtended = _pisock.dlpDBFlagExtended
dlpDBFlagFixedUp = _pisock.dlpDBFlagFixedUp
dlpDBMiscFlagExcludeFromSync = _pisock.dlpDBMiscFlagExcludeFromSync
dlpDBMiscFlagRamBased = _pisock.dlpDBMiscFlagRamBased
dlpRecAttrDeleted = _pisock.dlpRecAttrDeleted
dlpRecAttrDirty = _pisock.dlpRecAttrDirty
dlpRecAttrBusy = _pisock.dlpRecAttrBusy
dlpRecAttrSecret = _pisock.dlpRecAttrSecret
dlpRecAttrArchived = _pisock.dlpRecAttrArchived
dlpOpenRead = _pisock.dlpOpenRead
dlpOpenWrite = _pisock.dlpOpenWrite
dlpOpenExclusive = _pisock.dlpOpenExclusive
dlpOpenSecret = _pisock.dlpOpenSecret
dlpOpenReadWrite = _pisock.dlpOpenReadWrite
dlpEndCodeNormal = _pisock.dlpEndCodeNormal
dlpEndCodeOutOfMemory = _pisock.dlpEndCodeOutOfMemory
dlpEndCodeUserCan = _pisock.dlpEndCodeUserCan
dlpEndCodeOther = _pisock.dlpEndCodeOther
dlpDBListRAM = _pisock.dlpDBListRAM
dlpDBListROM = _pisock.dlpDBListROM
dlpDBListMultiple = _pisock.dlpDBListMultiple
dlpErrNoError = _pisock.dlpErrNoError
dlpErrSystem = _pisock.dlpErrSystem
dlpErrIllegalReq = _pisock.dlpErrIllegalReq
dlpErrMemory = _pisock.dlpErrMemory
dlpErrParam = _pisock.dlpErrParam
dlpErrNotFound = _pisock.dlpErrNotFound
dlpErrNoneOpen = _pisock.dlpErrNoneOpen
dlpErrAlreadyOpen = _pisock.dlpErrAlreadyOpen
dlpErrTooManyOpen = _pisock.dlpErrTooManyOpen
dlpErrExists = _pisock.dlpErrExists
dlpErrOpen = _pisock.dlpErrOpen
dlpErrDeleted = _pisock.dlpErrDeleted
dlpErrBusy = _pisock.dlpErrBusy
dlpErrNotSupp = _pisock.dlpErrNotSupp
dlpErrUnused1 = _pisock.dlpErrUnused1
dlpErrReadOnly = _pisock.dlpErrReadOnly
dlpErrSpace = _pisock.dlpErrSpace
dlpErrLimit = _pisock.dlpErrLimit
dlpErrSync = _pisock.dlpErrSync
dlpErrWrapper = _pisock.dlpErrWrapper
dlpErrArgument = _pisock.dlpErrArgument
dlpErrSize = _pisock.dlpErrSize
dlpErrUnknown = _pisock.dlpErrUnknown

dlp_GetSysDateTime = _pisock.dlp_GetSysDateTime

dlp_SetSysDateTime = _pisock.dlp_SetSysDateTime

dlp_ReadStorageInfo = _pisock.dlp_ReadStorageInfo

dlp_ReadSysInfo = _pisock.dlp_ReadSysInfo

dlp_ReadDBList = _pisock.dlp_ReadDBList

dlp_FindDBInfo = _pisock.dlp_FindDBInfo

dlp_OpenDB = _pisock.dlp_OpenDB

dlp_CloseDB = _pisock.dlp_CloseDB

dlp_CloseDB_All = _pisock.dlp_CloseDB_All

dlp_DeleteDB = _pisock.dlp_DeleteDB

dlp_CreateDB = _pisock.dlp_CreateDB

dlp_ResetSystem = _pisock.dlp_ResetSystem

dlp_AddSyncLogEntry = _pisock.dlp_AddSyncLogEntry

dlp_OpenConduit = _pisock.dlp_OpenConduit

dlp_EndOfSync = _pisock.dlp_EndOfSync

dlp_AbortSync = _pisock.dlp_AbortSync

dlp_ReadOpenDBInfo = _pisock.dlp_ReadOpenDBInfo

dlp_MoveCategory = _pisock.dlp_MoveCategory

dlp_WriteUserInfo = _pisock.dlp_WriteUserInfo

dlp_ReadUserInfo = _pisock.dlp_ReadUserInfo

dlp_ResetLastSyncPC = _pisock.dlp_ResetLastSyncPC

dlp_ReadAppBlock = _pisock.dlp_ReadAppBlock

dlp_WriteAppBlock = _pisock.dlp_WriteAppBlock

dlp_ReadSortBlock = _pisock.dlp_ReadSortBlock

dlp_WriteSortBlock = _pisock.dlp_WriteSortBlock

dlp_ResetDBIndex = _pisock.dlp_ResetDBIndex

dlp_WriteRecord = _pisock.dlp_WriteRecord

dlp_DeleteRecord = _pisock.dlp_DeleteRecord

dlp_DeleteCategory = _pisock.dlp_DeleteCategory

dlp_ReadResourceByType = _pisock.dlp_ReadResourceByType

dlp_ReadResourceByIndex = _pisock.dlp_ReadResourceByIndex

dlp_WriteResource = _pisock.dlp_WriteResource

dlp_DeleteResource = _pisock.dlp_DeleteResource

dlp_ReadNextModifiedRec = _pisock.dlp_ReadNextModifiedRec

dlp_ReadNextModifiedRecInCategory = _pisock.dlp_ReadNextModifiedRecInCategory

dlp_ReadNextRecInCategory = _pisock.dlp_ReadNextRecInCategory

dlp_ReadRecordById = _pisock.dlp_ReadRecordById

dlp_ReadRecordByIndex = _pisock.dlp_ReadRecordByIndex

dlp_CleanUpDatabase = _pisock.dlp_CleanUpDatabase

dlp_ResetSyncFlags = _pisock.dlp_ResetSyncFlags

dlp_ReadFeature = _pisock.dlp_ReadFeature

dlp_ReadNetSyncInfo = _pisock.dlp_ReadNetSyncInfo

dlp_WriteNetSyncInfo = _pisock.dlp_WriteNetSyncInfo

dlp_ReadAppPreference = _pisock.dlp_ReadAppPreference

dlp_WriteAppPreference = _pisock.dlp_WriteAppPreference

pi_file_open = _pisock.pi_file_open

pi_file_close = _pisock.pi_file_close

pi_file_get_info = _pisock.pi_file_get_info

pi_file_get_app_info = _pisock.pi_file_get_app_info

pi_file_get_sort_info = _pisock.pi_file_get_sort_info

pi_file_read_resource = _pisock.pi_file_read_resource

pi_file_read_resource_by_type_id = _pisock.pi_file_read_resource_by_type_id

pi_file_type_id_used = _pisock.pi_file_type_id_used

pi_file_read_record = _pisock.pi_file_read_record

pi_file_get_entries = _pisock.pi_file_get_entries

pi_file_read_record_by_id = _pisock.pi_file_read_record_by_id

pi_file_id_used = _pisock.pi_file_id_used

pi_file_create = _pisock.pi_file_create

pi_file_set_info = _pisock.pi_file_set_info

pi_file_set_app_info = _pisock.pi_file_set_app_info

pi_file_set_sort_info = _pisock.pi_file_set_sort_info

pi_file_append_resource = _pisock.pi_file_append_resource

pi_file_append_record = _pisock.pi_file_append_record

pi_file_retrieve = _pisock.pi_file_retrieve

pi_file_install = _pisock.pi_file_install

pi_file_merge = _pisock.pi_file_merge
dlp_ReadRecordIDList = _pisock.dlp_ReadRecordIDList


