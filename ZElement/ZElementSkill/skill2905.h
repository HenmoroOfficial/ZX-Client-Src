#ifndef __CPPGEN_GNET_SKILL2905
#define __CPPGEN_GNET_SKILL2905
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2905:public Skill
    {
      public:
        enum
        { SKILL_ID = 2905 };
          Skill2905 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2905Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2905Stub ():SkillStub (2905)
        {
            occupation = 151;
            name = L"°µÎÞÌìÈÕ";
            nativename = "°µÎÞÌìÈÕ";
            icon = "°µÎÞÌìÈÕ.dds";
            maxlevel = 14;
            maxlearn = 14;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
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
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
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
            action[0] = "¹íÍõ×Ú_ÆÆ¼×";
            action[1] = "ÇàÔÆÃÅ_ÆÆ¼×";
            action[2] = "¹íÍõ×Ú_ÆÆ¼×";
            action[3] = "ºÏ»¶ÅÉ_ÆÆ¼×";
            action[4] = "ÌìÒôËÂ_ÆÆ¼×";
            action[5] = "¹íµÀ_ÆÆ¼×";
            action[6] = "0";
            action[7] = "ÊÞÉñ_ÆÆ¼×";
            action[8] = "ºüÑý_ÆÆ¼×";
            action[9] = "0";
            action[10] = "»³¹â_ÆÆ¼×";
            action[11] = "0";
            action[12] = "Ìì»ª_ÆÆ¼×";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÆÆ¼×";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆ¼×";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÆÆ¼×";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆ¼×";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÆÆ¼×";
            action[20] = "¹íµÀ_Æï³Ë_ÆÆ¼×";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_ÆÆ¼×";
            action[23] = "ºüÑý_Æï³Ë_ÆÆ¼×";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_ÆÆ¼×";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_ÆÆ¼×";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
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
#endif
        }
        virtual ~ Skill2905Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 9);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               (skill->GetLevel () == 1 ? 0.8 : 0) + (skill->GetLevel () == 2 ? 1.3 : 0) + (skill->GetLevel () ==
                                                                                                            3 ? 1.8 : 0) + (skill->GetLevel () ==
                                                                                                                            4 ? 2.3 : 0) +
                               (skill->GetLevel () == 5 ? 4.7 : 0) + (skill->GetLevel () == 6 ? 5.5 : 0) + (skill->GetLevel () ==
                                                                                                            7 ? 6.3 : 0) + (skill->GetLevel () ==
                                                                                                                            8 ? 8.1 : 0) +
                               (skill->GetLevel () == 9 ? 8.9 : 0) + (skill->GetLevel () == 10 ? 9.7 : 0) + (skill->GetLevel () ==
                                                                                                             11 ? 10.5 : 0) + (skill->GetLevel () ==
                                                                                                                               12 ? 11.3 : 0) +
                               (skill->GetLevel () == 13 ? 14.1 : 0) + (skill->GetLevel () == 14 ? 16 : 0));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel (), 2000);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio ((skill->GetLevel () == 1 ? 0.008001 : 0) + (skill->GetLevel () == 2 ? 0.013001 : 0) +
                                           (skill->GetLevel () == 3 ? 0.018001 : 0) + (skill->GetLevel () ==
                                                                                       4 ? 0.023001 : 0) + (skill->GetLevel () ==
                                                                                                            5 ? 0.047001 : 0) + (skill->GetLevel () ==
                                                                                                                                 6 ? 0.055001 : 0) +
                                           (skill->GetLevel () == 7 ? 0.063001 : 0) + (skill->GetLevel () ==
                                                                                       8 ? 0.081001 : 0) + (skill->GetLevel () ==
                                                                                                            9 ? 0.089001 : 0) + (skill->GetLevel () ==
                                                                                                                                 10 ? 0.097001 : 0) +
                                           (skill->GetLevel () == 11 ? 0.105001 : 0) + (skill->GetLevel () ==
                                                                                        12 ? 0.113001 : 0) + (skill->GetLevel () ==
                                                                                                              13 ? 0.141001 : 0) +
                                           (skill->GetLevel () == 14 ? 0.16001 : 0));
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetDecskillaccu (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (300000);
            skill->GetVictim ()->SetRatio (2904);
            skill->GetVictim ()->SetAmount (2904);
            skill->GetVictim ()->SetValue (2904);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
