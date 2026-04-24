#ifndef __CPPGEN_GNET_SKILL1636
#define __CPPGEN_GNET_SKILL1636
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1636:public Skill
    {
      public:
        enum
        { SKILL_ID = 1636 };
          Skill1636 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1636Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0.5);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0.7);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0.9);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.1);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State6:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.3);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State7:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.5);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State8:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.7);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State9:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.9);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State10:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (2.1);
                skill->SetPlus (0.01 * skill->GetPlayer ()->GetHp () *
                                norm (skill->GetPlayer ()->GetHp () * 2 / (skill->GetPlayer ()->GetMaxhp () + 1)) +
                                0.01 * skill->GetPlayer ()->GetMp () * norm (skill->GetPlayer ()->GetMp () * 2 /
                                                                             (skill->GetPlayer ()->GetMaxmp () + 1)));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1636Stub ():SkillStub (1636)
        {
            occupation = 146;
            name = L"¾Å±ä£º°Ô";
            nativename = "¾Å±ä£º°Ô";
            icon = "¾Å±ä.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "¹íÍõ×Ú_¾Å±ä";
            action[1] = "¹íÍõ×Ú_¾Å±ä";
            action[2] = "¹íÍõ×Ú_¾Å±ä";
            action[3] = "¹íÍõ×Ú_¾Å±ä";
            action[4] = "¹íÍõ×Ú_¾Å±ä";
            action[5] = "¹íÍõ×Ú_¾Å±ä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¾Å±ä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¾Å±ä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¾Å±ä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¾Å±ä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¾Å±ä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¾Å±ä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
            statestub.push_back (new State6 ());
            statestub.push_back (new State7 ());
            statestub.push_back (new State8 ());
            statestub.push_back (new State9 ());
            statestub.push_back (new State10 ());
#endif
        }
        virtual ~ Skill1636Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (305);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 305);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetAmount (0.2 * skill->GetPlayer ()->GetDef ());
            skill->GetVictim ()->SetMpdisperse (1);
            return true;
        }
#endif
    };
}
#endif
