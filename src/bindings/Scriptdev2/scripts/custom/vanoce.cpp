#include "precompiled.h"

enum Spells
{
	SPELL_SNOWBLIND			= 64647,
	SPELL_SNOWFLAKES		= 39826,
	SPELL_SNOWBALL_SPRAY	= 44857,
	SPELL_SNOWDMGAURA		= 55799,
	SPELL_FROSTBOLT_VOLLEY	= 63758,
	SPELL_FROST_SPHERE		= 67539,

	SPELL_ICE_NOVA_P2		= 56935,
	SPELL_MINION_CHANNEL	= 60452,
	SPELL_AURA_STUN			= 47591,
	SPELL_BLIZZARD			= 58693,

	SPELL_BLUEFLAME			= 42571,
	SPELL_FROSTCHANELL		= 45846,

	SPELL_CAMERASHAKE		= 44762,
	SPELL_MINION_AURA_VIS	= 36151,

	SPELL_SHADOWFORM		= 55086,
	SPELL_SCREAM			= 41545,
	SPELL_SHADOWBOLT_VOLLEY	= 27831,

	SPELL_BANISH_STATE_1	= 54263,
	SPELL_BANISH_STATE_2	= 32567,
	SPELL_BANISH_STATE_3	= 32566,
	SPELL_BANISH_STATE_4	= 33343,
	SPELL_BANISH_STATE_5	= 33345,

	SPELL_FROSTBREATH		= 65248,
	SPELL_FROST_CLEAVE		= 51857,
	SPELL_PATH_FROST		= 61081
};

enum NPCS
{
	NPC_SMALLTERROR			= 99909,
	NPC_SOULIMAGE			= 99910
};

struct MANGOS_DLL_DECL vanocni_bossAI: public ScriptedAI
{
	vanocni_bossAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint8 phase;

	uint32 snowblind_timer;
	uint32 snowballspray_timer;
	uint32 visual_apply;
	uint32 frostbolt_timer;

	uint32 drain_timer;
	uint32 scream_timer;
	uint32 phase_timer;

	uint32 breath_timer;
	uint32 cleave_timer;

	Creature* spunti[15];
	Creature* souls[5];
	uint8 sphase;
	uint8 alivep;

	bool flakes;

	Unit* trg;

	bool special;
	uint32 specialrem;
	uint32 dmgt;

	void Reset()
	{
		m_creature->CombatStop();
		flakes = false;
		snowblind_timer = urand(4000,8000);
		snowballspray_timer = urand(15000,18000);
		frostbolt_timer = urand(8000,12000);
		drain_timer = 10000;
		visual_apply = 0;
		specialrem = 0;
		special = false;
		sphase = 0;
		phase = 1;
		dmgt = 0;
		trg = NULL;
		for(int i=0;i<14;++i)
			spunti[i] = NULL;
	}

	void Aggro(Unit* pWho)
	{
		m_creature->MonsterYell("Tak vy jste ti Ochranci Vanoc? Ha..ha..",LANG_UNIVERSAL,NULL);
	}

	void JustDied(Unit* pWho)
	{
		m_creature->MonsterYell("Kdyz Vanoce nezkazim ja.. zkazi je.. nekdo... jiny..",LANG_UNIVERSAL,NULL);
	}

	void CastFlakes()
	{
		Player* plmap[5];
		uint32  playercount = 0;
		uint32 cnt = 0;

		int i;
		for(i=0;i<5;++i)
			plmap[i] = NULL;

		Map *map = m_creature->GetMap();
		if(!map->IsDungeon()) return;
		if (map->IsDungeon())
		{
			Map::PlayerList const &PlayerList = map->GetPlayers();
			if (PlayerList.isEmpty())
				return;

			for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
			{
                if(m_creature->IsWithinDistInMap(i->getSource(), 100.0f))
				{
                    plmap[++cnt] = i->getSource();
				}
			}
		}

		playercount = cnt;
		for(cnt = 1;cnt < playercount+1;++cnt)
		{		
			if(plmap[cnt] && plmap[cnt]->isAlive())
			{
				m_creature->CastSpell(plmap[cnt],SPELL_SNOWFLAKES,false);
			}
		}
	}

	void SummonSpunty()
	{
		for(int i=0;i<15;++i)
		{
			spunti[i] = m_creature->SummonCreature(NPC_SMALLTERROR,0,0,0,0,TEMPSUMMON_DEAD_DESPAWN,0);
			if(spunti[i])
				spunti[i]->AI()->AttackStart(SelectUnit(SELECT_TARGET_RANDOM,0));
		} 
	}

	void UpdateAI(const uint32 diff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(!flakes && m_creature->GetHealth() < m_creature->GetMaxHealth()*0.05)
		{
			m_creature->InterruptNonMeleeSpells(false);
			phase = 10;
			CastFlakes();
			flakes = true;
		}

		if(phase == 1)
		{
		if(m_creature->GetHealth() < (m_creature->GetMaxHealth()/4)*3)
		{
			m_creature->InterruptNonMeleeSpells(false);
			DoCastSpellIfCan(m_creature,SPELL_ICE_NOVA_P2);
			specialrem = 6000;
			special = true;
			m_creature->MonsterYell("Santa Claus, Deda mraz, Jezisek.. co to znamena?",LANG_UNIVERSAL,NULL);
			++phase;
		}

		if(visual_apply)
		{
			if(visual_apply <= diff)
			{
				DoCastSpellIfCan(m_creature,SPELL_FROST_SPHERE);
				visual_apply = 0;
			} else visual_apply -= diff;
		}

		if(special)
		{
			if(specialrem < diff)
			{
				if(sphase == 1)
				{
					bool isalldead = true;
					for(int i=0;i<15;++i)
					{
						if(spunti[i] && spunti[i]->isAlive())
							isalldead = false;
					}
					if(isalldead)
					{
						m_creature->InterruptNonMeleeSpells(false);
						m_creature->RemoveAurasDueToSpell(SPELL_FROST_SPHERE);
						m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
						sphase = 0;
					} else {
						specialrem = 2000;
					}
				} 
				if(sphase == 0)
				{
					special = false;
					m_creature->InterruptNonMeleeSpells(false);
					m_creature->RemoveAurasDueToSpell(SPELL_SNOWDMGAURA);
					m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				}
			} else specialrem -= diff;
		}

		if(!special)
		{
		if(snowblind_timer < diff)
		{
			if(!m_creature->IsNonMeleeSpellCasted(false))
			{
				DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),SPELL_SNOWBLIND);
				snowblind_timer = urand(5000,8000);
			}
		} else snowblind_timer -= diff;

		if(snowballspray_timer < diff)
		{
			if(!m_creature->IsNonMeleeSpellCasted(false))
			{
				m_creature->GetMotionMaster()->MoveIdle();				
				specialrem = 8000;
				visual_apply = 100+diff;
				sphase = 0;
				special = true;
				snowballspray_timer = urand(30000,45000);
				m_creature->CastSpell(m_creature,SPELL_SNOWDMGAURA,false);
				sphase = 1;
				SummonSpunty();
			}
		} else snowballspray_timer -= diff;

		if(frostbolt_timer < diff)
		{
			if(!m_creature->IsNonMeleeSpellCasted(false))
			{
				m_creature->CastSpell(m_creature->getVictim(),SPELL_FROSTBOLT_VOLLEY,false);
				frostbolt_timer = urand(6000,8000);
			}
		} else frostbolt_timer -= diff;

		DoMeleeAttackIfReady();
		}
		}

		if(phase == 2)
		{
			if(drain_timer < diff)
			{
				m_creature->DealDamage(trg,1200,0,DOT,SPELL_SCHOOL_MASK_ALL,0,false);
				m_creature->SetHealth((m_creature->GetHealth()+1200));
				drain_timer = 1000;
			} else drain_timer -= diff;
			if(visual_apply)
			{
				if(visual_apply < diff)
				{
					m_creature->CastSpell(trg,SPELL_MINION_CHANNEL,false);
					visual_apply = 0;
				} else visual_apply -= diff;
			}
			if(specialrem && special)
			{
				if(specialrem <= diff)
				{
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->CastSpell(m_creature,SPELL_SNOWDMGAURA,false);
					trg = SelectUnit(SELECT_TARGET_RANDOM,0);
					trg->InterruptNonMeleeSpells(false);
					trg->CastSpell(trg,SPELL_AURA_STUN,false);
					m_creature->CastSpell(trg,SPELL_BLIZZARD,false);
					float px,py,pz;
					px = trg->GetPositionX();
					py = trg->GetPositionY();
					pz = trg->GetPositionZ();
					specialrem = 10000;
					special = false;
					dmgt = 0;
					visual_apply = 200;
					drain_timer = 1000;
				} else specialrem -= diff;
			}
			if(specialrem && !special)
			{
				//
			}
			if(!specialrem && !special)
			{
				m_creature->MonsterYell("Ano, darky kradu ja!",0,NULL);
				m_creature->RemoveAllAuras();
				m_creature->InterruptNonMeleeSpells(false);
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				m_creature->CastSpell(m_creature,SPELL_CAMERASHAKE,false);
				m_creature->CastSpell(m_creature,SPELL_SHADOWFORM,false);
				scream_timer = 3000;
				frostbolt_timer = 8500;
				phase_timer = 30000;
				++phase;
			}
			DoMeleeAttackIfReady();
		}
		if(phase == 3)
		{
			if(phase_timer < diff)
			{
				m_creature->InterruptNonMeleeSpells(false);
				m_creature->GetMotionMaster()->MovementExpired(false);
				m_creature->GetMotionMaster()->MoveIdle();
				m_creature->CastSpell(m_creature,SPELL_BANISH_STATE_1,false);
				for(int i=0;i<5;++i)
				{
					souls[i] = m_creature->SummonCreature(NPC_SOULIMAGE,0,0,0,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,40000);
					if(souls[i])
					{
						souls[i]->AI()->AttackStart(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
						souls[i]->CastSpell(souls[i],SPELL_MINION_AURA_VIS,false);
					}
				}
				alivep = 5;
				sphase = 0;
				++phase;
			} else phase_timer -= diff;
			if(scream_timer < diff)
			{
				if(!m_creature->IsNonMeleeSpellCasted(false))
				{
					m_creature->CastSpell(m_creature->getVictim(),SPELL_SCREAM,false);
					scream_timer = urand(4000,6000);
				}
			} else scream_timer -= diff;

			if(frostbolt_timer < diff)
			{
				if(!m_creature->IsNonMeleeSpellCasted(false))
				{
					m_creature->CastSpell(m_creature->getVictim(),SPELL_SHADOWBOLT_VOLLEY,false);
					frostbolt_timer = urand(6000,8000);
				}
			} else frostbolt_timer -= diff;
			DoMeleeAttackIfReady();
		}
		if(phase == 4)
		{
			uint8 alivem = 5;
			for(int i=0;i<5;++i)
				if(souls[i] && souls[i]->isDead())
					--alivem;
			if(alivem < alivep)
			{
				switch(alivem)
				{
				case 4:
					m_creature->RemoveAurasDueToSpell(SPELL_BANISH_STATE_1);
					m_creature->CastSpell(m_creature,SPELL_BANISH_STATE_2,false);
					alivep = alivem;
					break;
				case 3:
					m_creature->RemoveAurasDueToSpell(SPELL_BANISH_STATE_2);
					m_creature->CastSpell(m_creature,SPELL_BANISH_STATE_3,false);
					alivep = alivem;
					break;
				case 2:
					m_creature->RemoveAurasDueToSpell(SPELL_BANISH_STATE_3);
					m_creature->CastSpell(m_creature,SPELL_BANISH_STATE_4,false);
					alivep = alivem;
					break;
				case 1:
					m_creature->RemoveAurasDueToSpell(SPELL_BANISH_STATE_4);
					m_creature->CastSpell(m_creature,SPELL_BANISH_STATE_5,false);
					alivep = alivem;
					break;
				default:
					m_creature->RemoveAurasDueToSpell(SPELL_BANISH_STATE_5);
					m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
					alivep = alivem;
					breath_timer = urand(1000,3000);
					cleave_timer = urand(8000,9000);
					m_creature->CastSpell(m_creature,SPELL_PATH_FROST,false);
					m_creature->MonsterYell("Tak to rychle ukoncime!",LANG_UNIVERSAL,NULL);
					++phase;
					break;
				}
			}
			DoMeleeAttackIfReady();
		}
		if(phase == 5)
		{
			if(breath_timer < diff)
			{
				if(!m_creature->IsNonMeleeSpellCasted(false))
				{
					m_creature->CastSpell(m_creature->getVictim(),SPELL_FROSTBREATH,false);
					breath_timer = urand(4000,6000);
				}
			} else breath_timer -= diff;

			if(cleave_timer < diff)
			{
				if(!m_creature->IsNonMeleeSpellCasted(false))
				{
					m_creature->CastSpell(m_creature->getVictim(),SPELL_FROST_CLEAVE,false);
					cleave_timer = urand(8000,9000);
				}
			} else cleave_timer -= diff;
			DoMeleeAttackIfReady();
		}
	}

	void DamageTaken(Unit* pWho, uint32 &damage)
	{
		if(phase == 2 && specialrem)
		{
			dmgt += damage;
			if(dmgt > 30000)
			{
				trg->RemoveAurasDueToSpell(SPELL_AURA_STUN);
				m_creature->RemoveAurasDueToSpell(SPELL_FROSTCHANELL);
				m_creature->RemoveAurasDueToSpell(SPELL_MINION_CHANNEL);
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				specialrem = 0;
			}
		}
	}
};

CreatureAI* GetAI_vanocni_boss(Creature* pCreature)
{
	return new vanocni_bossAI(pCreature);
}

struct MANGOS_DLL_DECL frozensoul_vanoceAI: public ScriptedAI
{
	frozensoul_vanoceAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	void Reset() {}
	void Aggro(Unit* ) { return; }
	void MoveInLineOfSight(Unit* ) { return; }
	void AttackStart(Unit* ) { return; }
};

CreatureAI* GetAI_frozensoul_vanoce(Creature* pCreature)
{
	return new frozensoul_vanoceAI(pCreature);
}

struct MANGOS_DLL_DECL emoter_vanoceAI: public ScriptedAI
{
	emoter_vanoceAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 emote_timer;
	uint32 uses;
	uint8 em_type;
	Player* pp;
	bool sleeps;

	void Reset()
	{
		emote_timer = 0;
		em_type = 0;
		uses = 0;
		sleeps = false;
	}

	bool HasSpecialAura(Player* who)
	{
		if (who->HasAura(46740)) return true;
		if (who->HasAura(46739)) return true;
		if (who->HasAura(46738)) return true;
		if (who->HasAura(46737)) return true;
		if (who->HasAura(46736)) return true;
		if (who->HasAura(3329)) return true;
		if (who->HasAura(25858)) return true;
		if (who->HasAura(21936)) return true;
		return false;
	}

	uint32 SelectAura()
	{
		uint32 selspell;
		switch(urand(0,14))
		{
		case 0:
		case 6:
			selspell = 46740;
			break;
		case 1:
		case 7:
			selspell = 46739;
			break;
		case 2:
		case 8:
			selspell = 46738;
			break;
		case 3:
		case 9:
			selspell = 46737;
			break;
		case 4:
		case 10:
			selspell = 46736;
			break;
		case 5:
		case 11:
			selspell = 21936;
			break;
		case 12:
			if(urand(0,1))
				selspell = 44824;
			else
				selspell = 3329;
			break;
		case 13:
			if(urand(0,1))
				selspell = 55164;
			else
				selspell = 3329;
			break;
		default:
			selspell = 25858;
			break;
		}
		return selspell;
	}

	void ReceiveEmote(Player* who,uint32 emoteid)
	{
		if(sleeps) return;
		if(emote_timer) return;
		if (emoteid == TEXTEMOTE_KISS)
		{
			em_type = 0;
			pp = who;
			emote_timer = 1500;
			++uses;
		}
		if (emoteid == TEXTEMOTE_RUDE)
		{
			em_type = 1;
			pp = who;
			emote_timer = 1500;
			++uses;
		}
	}

	void UpdateAI(const uint32 diff)
	{
		if(sleeps)
		{
			if(emote_timer < diff)
			{
				m_creature->HandleEmoteCommand(EMOTE_STATE_STAND);
				m_creature->SetStandState(UNIT_STAND_STATE_STAND);
				m_creature->RemoveAurasDueToSpell(55474);
				sleeps = false;
				emote_timer = 0;
			} else emote_timer -= diff;
		}
		if(emote_timer && !sleeps)
		{
			if(emote_timer < diff)
			{
				emote_timer = 0;
				if(em_type == 0)
				{
					if(!HasSpecialAura(pp))
					{
						pp->CastSpell(pp,SelectAura(),false);
						m_creature->HandleEmoteCommand(EMOTE_ONESHOT_SHY);
					}
				}
				if(em_type == 1)
				{
					m_creature->CastSpell(m_creature,62013,false);
					pp->CastSpell(pp,5,false); //36877
					m_creature->HandleEmoteCommand(EMOTE_ONESHOT_CRY);
				}
			} else emote_timer -= diff;
		}
		if(uses > 20)
		{
			sleeps = true;
			m_creature->CastSpell(m_creature,55474,false);
			m_creature->HandleEmoteCommand(EMOTE_STATE_SLEEP);
			m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
			emote_timer = 60000;
			uses = 0;
		}
	}
};

CreatureAI* GetAI_emoter_vanoce(Creature* pCreature)
{
	return new emoter_vanoceAI(pCreature);
}

void AddSC_vanocni_boss()
{
	Script *newscript;

	newscript = new Script;
	newscript->Name = "vanocni_boss";
	newscript->GetAI = &GetAI_vanocni_boss;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "frozensoul_vanoce";
	newscript->GetAI = &GetAI_frozensoul_vanoce;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "emoter_vanoce";
	newscript->GetAI = &GetAI_emoter_vanoce;
	newscript->RegisterSelf();
}