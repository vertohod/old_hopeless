#ifndef COMMANDS_H
#define COMMANDS_H

namespace chatd
{

enum COMMANDS : int
{
    COMMAND_BEGIN               = 1,
    RESULT						= 1,
    AUTHENTICATION_REQUEST      = 2,
    CHANGE_NICKNAME_REQUEST     = 3,
    LOGOUT                      = 4,
    REMOVE_LOGIN                = 5,
    COME_IN                     = 6,
    GET_OUT                     = 7,
    CMESSAGE                    = 8,
    COMMAND_END                 = 8
};

} // end of namespace
#endif
