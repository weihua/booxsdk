#include "onyx/ui/spanish_keyboard_data.h"

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
    'm','p','.',};

SpanishKeyboardData::SpanishKeyboardData()
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

SpanishKeyboardData::~SpanishKeyboardData()
{
}

// for initialization
void SpanishKeyboardData::initTopKeyCode()
{
    const QChar chs[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    for (int i = 0; i < 10; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        top_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initLeftKeyCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_LEFT[i])));
        left_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initMiddleKeyCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_MIDDLE[i])));
        middle_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initRightKeyCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_RIGHT[i])));
        right_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initBottomKeyCode()
{
    /*
  chars['a acute'] = { lower: '\u00E1', upper: '\u00C1' };
  chars['e acute'] = { lower: '\u00E9', upper: '\u00C9' };
  chars['i acute'] = { lower: '\u00ED', upper: '\u00CD' };
  chars['n tilde'] = { lower: '\u00F1', upper: '\u00D1' };
  chars['o acute'] = { lower: '\u00F3', upper: '\u00D3' };

  chars['u acute'] = { lower: '\u00FA', upper: '\u00DA' };
  chars['u uml'] = { lower: '\u00FC', upper: '\u00DC' }
    */

    const QChar chs[] = {QChar(0x00e1),QChar(0x00e9), QChar(0x00ed),QChar(0x00f1),QChar(0x00f3),
            };
    for (int i=0; i<5; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        bottom_codes_.push_back(dd);
    }

    bottom_codes_.push_back(ODataPtr(createSpaceData()));
    ODataPtr dd(createBackspaceData());
    bottom_codes_.push_back(dd);

    const QChar chs_next[] = {QChar(0x00fa),QChar(0x00FC) };
    for (int i=0; i<2; i++)
    {
        ODataPtr dd(createData(QString(chs_next[i])));
        bottom_codes_.push_back(dd);
    }

    bottom_codes_.push_back(createData(QString('@')));
}

// for shift key
void SpanishKeyboardData::initTopKeyShiftCode()
{
    const QChar chs[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')'};
    for (int i = 0; i < 10; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        top_shift_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initLeftKeyShiftCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd (createData(QString(SPANISH_LEFT[i].toUpper())));
        left_shift_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initMiddleKeyShiftCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd (createData(QString(SPANISH_MIDDLE[i].toUpper())));
        middle_shift_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initRightKeyShiftCode()
{
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(SPANISH_RIGHT[i].toUpper())));
        right_shift_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initBottomKeyShiftCode()
{
    const QChar chs[] = {QChar(0x00c1),QChar(0x00c9), QChar(0x00cd), QChar(0x0d1),
            QChar(0x00d3)};
    for (int i=0; i<5; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        bottom_shift_codes_.push_back(dd);
    }

    bottom_shift_codes_.push_back(ODataPtr(createSpaceData()));
    ODataPtr dd(createBackspaceData());
    bottom_shift_codes_.push_back(dd);

    const QChar chs_next[] = {QChar(0x00da), QChar(0x00dc)};
    for (int i=0; i<2; i++)
    {
        ODataPtr dd (createData(QString(chs_next[i])));
        bottom_shift_codes_.push_back(dd);
    }

    bottom_shift_codes_.push_back(ODataPtr(createEnterData()));
}

// for symbol key
void SpanishKeyboardData::initLeftKeySymbolCode()
{

    /*
  chars['inverted ?'] = { lower: '\u00BF' };
  chars['inverted !'] = { lower: '\u00A1' };
  chars['apostrophe'] = { lower: '\u2019' };
  chars['dash'] = { lower: '-' };
      */
    const QChar chs[] = {'.', ',', '\'',
                         QChar(0x00bf), QChar(0x00a1), QChar(0x2019),
                         '-','[',']' };
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd(createData(QString(chs[i])));
        left_symbol_codes_.push_back(dd);
    }
}

void SpanishKeyboardData::initMiddleKeySymbolCode()
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

void SpanishKeyboardData::initRightKeySymbolCode()
{
    /*
  chars['high inverted quote'] = { lower: '\u201C' };
  chars['high quote'] = { lower: '\u201D' };
  chars['<<'] = { lower: '\u00AB' };
  chars['>>'] = { lower: '\u00BB' };
  chars['endash'] = { lower: '\u2013' };
  chars['emdash'] = { lower: '\u2014' };
      */
    const QChar chs[] = {QChar(0x201c), QChar(0x201d), QChar(0x00ab),
                         QChar(0x00bb), '<', '>',
                         '|', QChar(0x2013), QChar(0x2014),
                         };
    for (int i = 0; i < 9; i++)
    {
        ODataPtr dd( createData(QString(chs[i])));
        right_symbol_codes_.push_back(dd);
    }
}

}
