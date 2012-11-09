/*
 * catalan_keyboard_data.cpp
 *
 *  Created on: 2012-9-2
 *      Author: weihuahuang
 */


#include "onyx/ui/catalan_keyboard_data.h"

namespace ui
{

static const QChar SPANISH_LEFT[] = {
    'q','w','e',
    'a','s','d',
    'z','x','c',};

static const QChar SPANISH_MIDDLE[] = {
    'r','t','y',
    'f','g','h',
    'v','b','n',};
static const QChar SPANISH_RIGHT[] = {
    'u','i','o',
    'j','k','l',
    'm','p',};

CatalanKeyboardData::CatalanKeyboardData()
    : KeyboardData()
{
    initTopKeyCode();
    initLeftKeyCode();
    initMiddleKeyCode();
    initRightKeyCode();
    initBottomKeyCode();

    initTopKeyShiftCode();
    initLeftKeyShiftCode();
    initMiddleKeyShiftCode();
    initRightKeyShiftCode();
    initBottomKeyShiftCode();

    initLeftKeySymbolCode();
    initMiddleKeySymbolCode();
    initRightKeySymbolCode();
}

CatalanKeyboardData::~CatalanKeyboardData()
{
}

// for initialization
void CatalanKeyboardData::initTopKeyCode()
{
    const QChar chs[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    for (int i = 0; i < 10; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        top_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initLeftKeyCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_LEFT[i])));
        left_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initMiddleKeyCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_MIDDLE[i])));
        middle_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initRightKeyCode()
{
    for (int i = 0; i < 8; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_RIGHT[i])));
        right_codes_.push_back(dd);
    }
    ODataPtr dd(createData(QString(QChar(0x00e7))));
    right_codes_.push_back(dd);
}

void CatalanKeyboardData::initBottomKeyCode()
{
    const QChar chs[] = {QChar(0x00e0), QChar(0x00e9),QChar(0x00e8),QChar(0x00ed),QChar(0x00ef),
            };
    for (int i=0; i<5; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        bottom_codes_.push_back(dd);
    }

    bottom_codes_.push_back(ODataPtr(createSpaceData()));
    ODataPtr dd(createBackspaceData());
    bottom_codes_.push_back(dd);

    const QChar chs_next[] = {'.',QChar(0x00FC) };
    for (int i=0; i<2; i++)
    {
        ODataPtr dd(createData(QString(chs_next[i])));
        bottom_codes_.push_back(dd);
    }

    bottom_codes_.push_back(createData(QString('@')));
}

// for shift key
void CatalanKeyboardData::initTopKeyShiftCode()
{
    const QChar chs[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')'};
    for (int i = 0; i < 10; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        top_shift_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initLeftKeyShiftCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd (createData(QString(SPANISH_LEFT[i].toUpper())));
        left_shift_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initMiddleKeyShiftCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd (createData(QString(SPANISH_MIDDLE[i].toUpper())));
        middle_shift_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initRightKeyShiftCode()
{
    for (int i = 0; i < 8; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_RIGHT[i].toUpper())));
        right_shift_codes_.push_back(dd);
    }
    ODataPtr dd(createData(QString(QChar(0x00e7).toUpper())));
    right_shift_codes_.push_back(dd);
}

void CatalanKeyboardData::initBottomKeyShiftCode()
{
    const QChar chs[] = {QChar(0x00e0), QChar(0x00e9),QChar(0x00e8),QChar(0x00ed),QChar(0x00ef),
            };
    for (int i=0; i<5; i++)
    {
        ODataPtr dd(createData(QString(chs[i].toUpper())));
        bottom_shift_codes_.push_back(dd);
    }

    bottom_shift_codes_.push_back(ODataPtr(createSpaceData()));
    ODataPtr dd(createBackspaceData());
    bottom_shift_codes_.push_back(dd);

    const QChar chs_next[] = {'.', QChar(0x00dc)};
    for (int i=0; i<2; i++)
    {
        ODataPtr dd (createData(QString(chs_next[i])));
        bottom_shift_codes_.push_back(dd);
    }

    bottom_shift_codes_.push_back(ODataPtr(createEnterData()));
}

// for symbol key
void CatalanKeyboardData::initLeftKeySymbolCode()
{
    const QChar chs[] = { QChar(0x00a1), QChar(0x00f1), QChar(0x00f1).toUpper(),
                         QChar(0x00bf), QChar(0x00f2), QChar(0x00f2).toUpper(),
                         ',', QChar(0x00fa), QChar(0x00fa).toUpper()};
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        left_symbol_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initMiddleKeySymbolCode()
{
    const QChar chs[] = {'+', '-', '?',
                         ':', '"', '=',
                         '_', '{', '}', };
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd (createData(QString(chs[i])));
        middle_symbol_codes_.push_back(dd);
    }
}

void CatalanKeyboardData::initRightKeySymbolCode()
{
    const QChar chs[] = {QChar(0x201c),  '[', ']',
                         QChar(0x201d), '<', '>',
                         '|', QChar(0x2013), QChar(0x2014),
                         };
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd( createData(QString(chs[i])));
        right_symbol_codes_.push_back(dd);
    }
}

}

