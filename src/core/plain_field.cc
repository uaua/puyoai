#include "core/plain_field.h"

#include <sstream>

#include "core/field_bit_field.h"
#include "core/position.h"
#include "core/score.h"

using namespace std;

PlainField::PlainField()
{
    initialize();
}

PlainField::PlainField(const string& url)
{
    initialize();

    string prefix = "http://www.inosendo.com/puyo/rensim/??";
    int dataStartsAt = url.find(prefix) == 0 ? prefix.length() : 0;

    int counter = 0;
    for (int i = url.length() - 1; i >= dataStartsAt; --i) {
        int x = 6 - (counter % 6);
        int y = counter / 6 + 1;
        PuyoColor c = toPuyoColor(url[i]);
        unsafeSet(x, y, c);
        counter++;
    }
}

void PlainField::initialize()
{
    // Initialize field information.
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y)
            unsafeSet(x, y, PuyoColor::EMPTY);
    }

    for (int x = 0; x < MAP_WIDTH; ++x) {
        unsafeSet(x, 0, PuyoColor::WALL);
        unsafeSet(x, MAP_HEIGHT - 1, PuyoColor::WALL);
    }

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        unsafeSet(0, y, PuyoColor::WALL);
        unsafeSet(MAP_WIDTH - 1, y, PuyoColor::WALL);
    }
}

bool PlainField::isZenkeshiPrecise() const
{
    for (int x = 1; x <= WIDTH; ++x) {
        for (int y = 1; y <= 13; ++y) {
            if (!isEmpty(x, y))
                return false;
        }
    }

    return true;
}

Position* PlainField::fillSameColorPosition(int x, int y, PuyoColor c,
                                            Position* positionQueueHead, FieldBitField* checked) const
{
    DCHECK(!checked->get(x, y));

    if (FieldConstant::HEIGHT < y)
        return positionQueueHead;

    Position* writeHead = positionQueueHead;
    Position* readHead = positionQueueHead;

    *writeHead++ = Position(x, y);
    checked->set(x, y);

    while (readHead != writeHead) {
        Position p = *readHead++;

        if (color(p.x + 1, p.y) == c && !checked->get(p.x + 1, p.y)) {
            *writeHead++ = Position(p.x + 1, p.y);
            checked->set(p.x + 1, p.y);
        }
        if (color(p.x - 1, p.y) == c && !checked->get(p.x - 1, p.y)) {
            *writeHead++ = Position(p.x - 1, p.y);
            checked->set(p.x - 1, p.y);
        }
        if (color(p.x, p.y + 1) == c && !checked->get(p.x, p.y + 1) && p.y + 1 <= FieldConstant::HEIGHT) {
            *writeHead++ = Position(p.x, p.y + 1);
            checked->set(p.x, p.y + 1);
        }
        if (color(p.x, p.y - 1) == c && !checked->get(p.x, p.y - 1)) {
            *writeHead++ = Position(p.x, p.y - 1);
            checked->set(p.x, p.y - 1);
        }
    }

    return writeHead;
}

int PlainField::vanishSlow(int currentChain)
{
    FieldBitField checked;
    Position eraseQueue[WIDTH * HEIGHT]; // All the positions of erased puyos will be stored here.
    Position* eraseQueueHead = eraseQueue;

    bool usedColors[NUM_PUYO_COLORS] {};
    int numUsedColors = 0;
    int longBonusCoef = 0;

    for (int x = 1; x <= WIDTH; ++x) {
        for (int y = 1; y <= HEIGHT; ++y) {
            if (isEmpty(x, y))
                continue;

            if (checked.get(x, y))
                continue;
            if (!isNormalColor(color(x, y)))
                continue;

            PuyoColor c = color(x, y);
            Position* head = fillSameColorPosition(x, y, c, eraseQueueHead, &checked);

            int connectedPuyoNum = head - eraseQueueHead;
            if (connectedPuyoNum < PUYO_ERASE_NUM)
                continue;

            eraseQueueHead = head;
            longBonusCoef += longBonus(connectedPuyoNum);
            if (!usedColors[static_cast<int>(c)]) {
                ++numUsedColors;
                usedColors[static_cast<int>(c)] = true;
            }
        }
    }

    int numErasedPuyos = eraseQueueHead - eraseQueue;
    if (numErasedPuyos == 0)
        return 0;

    // --- Actually erase the Puyos to be vanished. We erase ojama here also.
    for (Position* head = eraseQueue; head != eraseQueueHead; ++head) {
        int x = head->x;
        int y = head->y;

        unsafeSet(x, y, PuyoColor::EMPTY);

        // Check OJAMA puyos erased
        if (color(x + 1, y) == PuyoColor::OJAMA) {
            unsafeSet(x + 1, y, PuyoColor::EMPTY);
        }

        if (color(x - 1, y) == PuyoColor::OJAMA) {
            unsafeSet(x - 1, y, PuyoColor::EMPTY);
        }

        // We don't need to update minHeights here.
        if (color(x, y + 1) == PuyoColor::OJAMA && y + 1 <= HEIGHT) {
            unsafeSet(x, y + 1, PuyoColor::EMPTY);
        }

        if (color(x, y - 1) == PuyoColor::OJAMA) {
            unsafeSet(x, y - 1, PuyoColor::EMPTY);
        }
    }

    int rensaBonusCoef = calculateRensaBonusCoef(chainBonus(currentChain), longBonusCoef, colorBonus(numUsedColors));
    return 10 * numErasedPuyos * rensaBonusCoef;
}

string PlainField::toString(char charIfEmpty) const
{
    ostringstream ss;
    for (int y = 14; y >= 1; --y) {
        for (int x = 1; x <= WIDTH; ++x) {
            ss << toChar(color(x, y), charIfEmpty);
        }
    }

    return ss.str();
}

bool operator==(const PlainField& lhs, const PlainField& rhs)
{
    for (int x = 1; x <= FieldConstant::WIDTH; ++x) {
        for (int y = 1; y <= FieldConstant::HEIGHT; ++y) {
            if (lhs.color(x, y) != rhs.color(x, y))
                return false;
        }
    }

    return true;
}
