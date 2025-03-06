#pragma once
#ifdef XBOX_CONSOLE_TYPE_H
#define XBOX_CONSOLE_TYPE_H

typedef enum XboxConsoleType
{
    DevelopmentKit,
    TestKit,
    ReviewerKit,
    NotConnected
} XBOX_CONSOLE_TYPE;
#endif // XBOX_CONSOLE_TYPE_H
