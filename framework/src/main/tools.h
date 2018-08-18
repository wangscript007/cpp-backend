/**
 * @file tools.h
 * @author Feo
 * @date 10/08/2018
 * @brief
 */

#ifndef FRAMEWORK_TOOLS_H
#define FRAMEWORK_TOOLS_H

#define FW_COUNTER          __COUNTER__

#define FW_STR_IMPL(str)    #str
#define FW_STR(str)         FW_STR_IMPL(str)

#define FW_COMMA()          ,

#define FW_FORMAT_BOOL(b)   (b ? "true" : "false")

#define FW_ULOCK(n, m)      std::unique_lock<decltype(m)> n(m)

#define FW_UNUSED           [[maybe_unused]]

#endif /* FRAMEWORK_TOOLS_H */
