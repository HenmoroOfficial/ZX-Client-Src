#ifndef __CPPGEN_GNET_SKILL3734
#define __CPPGEN_GNET_SKILL3734
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3734:public Skill
    {
      public:
        enum
        { SKILL_ID = 3734 };
          Skill3734 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3734Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 3000;
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
                return 5000;
            }
            void Calculate (Skill * skill) const
            {
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3734Stub ():SkillStub (3734)
        {
            occupation = 128;
            name = L"°ËĞ×Ğş»ğ";
            nativename = "°ËĞ×Ğş»ğ";
            icon = "°ËĞ×Ğş»ğ¡¤Õó·¨.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 1;
            cyclegfx = "·¨Õó°ËĞ×";
            cyclemode = 1;
            skill_class = -2;
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
            talent[0] = 3421;
            talent[1] = 3422;
            talent[2] = 3423;
            talent[3] = 3408;
            talent_size = 4;
            action[0] = "·ÙÏã¹È_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[1] = "·ÙÏã¹È_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[2] = "·ÙÏã¹È_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[3] = "·ÙÏã¹È_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[4] = "·ÙÏã¹È_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[5] = "·ÙÏã¹È_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[6] = "·ÙÏã¹È_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[16] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[17] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[18] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[19] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[20] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[21] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑĞ¡¾°ËĞ×¡¿";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 2;
            doenchant = true;
            dobless = true;
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
#endif
        }
        virtual ~ Skill3734Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 5000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 15000 * 3;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (28);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, (20 + 5 * 3), 30, 10);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (20 * 2));
            skill->GetVictim ()->SetBreakcasting (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000 * norm (3) * skill->GetPlayer ()->GetCyclemembercnt () * 3);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (3608);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetCycle (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (50);
            skill->GetVictim ()->
                SetRatio (int
                          ((skill->GetCharging () > 4500
                            && skill->GetPlayer ()->GetCyclemembercnt () == 6) ? 3 : ((skill->GetCharging () > 2500
                                                                                       && skill->GetPlayer ()->GetCyclemembercnt () >
                                                                                       3) ? 2 : 1)) * 100 + (20 + 5 * 3 +
                                                                                                             skill->GetPlayer ()->
                                                                                                             GetCyclemembercnt () * 3 +
                                                                                                             10 * int (skill->GetCharging () >
                                                                                                                       4800)));
              skill->GetVictim ()->SetBuffid (1);
              skill->GetVictim ()->SetValue (3646);
              skill->GetVictim ()->SetDelaycast (1);
              skill->GetVictim ()->SetTime (900);
              skill->GetVictim ()->SetRatio (0);
              skill->GetVictim ()->SetAmount (0);
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetEvilaura (1);
              return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6);
        }
#endif
    };
}
#endif
