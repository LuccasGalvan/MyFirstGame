// pch.h: esse é um arquivo de cabeçalho pré-compilado.
// Os arquivos listados a seguir são compilados somente uma vez, melhorando o desempenho dos builds futuros.
// Isso também afeta o desempenho do IntelliSense, incluindo a conclusão de código e muitos recursos de navegação de código.
// No entanto, os arquivos listados aqui serão TODOS recompilados se qualquer um deles for atualizado entre builds.
// Não adicione aqui arquivos que você atualizará com frequência, pois isso anula a vantagem de desempenho.

#ifndef PCH_H
#define PCH_H

// adicione os cabeçalhos que você deseja pré-compilar aqui

#include "MyRPGLib/pc_macros.h"
#include "MyRPGLib/ability.h"
#include "MyRPGLib/armor.h"
#include "MyRPGLib/berserker.h"
#include "MyRPGLib/buff.h"
#include "MyRPGLib/cleric.h"
#include "MyRPGLib/corestats.h"
#include "MyRPGLib/item.h"
#include "MyRPGLib/item_manager.h"
#include "MyRPGLib/playableclasses_fwd.h"
#include "MyRPGLib/playerCharacter.h"
#include "MyRPGLib/pointwell.h"
#include "MyRPGLib/potion.h"
#include "MyRPGLib/rogue.h"
#include "MyRPGLib/statblock.h"
#include "MyRPGLib/types.h"
#include "MyRPGLib/warrior.h"
#include "MyRPGLib/weapon.h"
#include "MyRPGLib/monster.h"
#include "MyRPGLib/random.h"


#endif //PCH_H
