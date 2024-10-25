#pragma once

#define PCCONSTRUCT \
HP->SetMax(BASEHP);\
HP->IncreaseCurrent(BASEHP);\
if (MP) { \
MP->SetMax(BASEMP); \
MP->IncreaseCurrent(BASEMP); \
}\
if (EP) { \
EP->SetMax(BASEEP); \
EP->IncreaseCurrent(BASEEP); \
}\
if (RP) { \
RP->SetMax(100); \
RP->IncreaseCurrent(BASERP); \
}\
increase_stats(BASESTR, BASEINT, BASEAGI);

#define LEVELUP \
HP->SetMax((welltype)((BASEHP / 2.f) + HP->GetMax()));\
HP->IncreaseCurrent((welltype)(BASEHP / 2.f));\
if (MP) {\
MP->SetMax((welltype)((BASEMP / 2.f) + MP->GetMax()));\
MP->IncreaseCurrent((welltype)(BASEMP / 2.f));\
}\
if (EP) {\
EP->SetMax((welltype)((BASEEP / 2.f) + EP->GetMax()));\
EP->IncreaseCurrent((welltype)(BASEEP / 2.f));\
}\
increase_stats((statustype)((BASESTR + 1u) / 2.f), (statustype)((BASEINT + 1u) / 2.f), (statustype)((BASEAGI + 1u) / 2.f));