#ifndef __CPPGEN_GNET_SKILL2069
#define __CPPGEN_GNET_SKILL2069
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2069:public Skill
    {
      public:
        enum
        { SKILL_ID = 2069 };
          Skill2069 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2069Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (460 + 45 * skill->GetLevel ()) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (2.5 * skill->GetLevel () * skill->GetLevel () + 45 * skill->GetLevel () +
                                                                        300)));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2069Stub ():SkillStub (2069)
        {
            occupation = 150;
            name = L"ÎÞÍýÕ¶II";
            nativename = "ÎÞÍýÕ¶II";
            icon = "ÎÞÍýÕ¶II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
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
            preskillnum = 1;
            action[0] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[1] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[2] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[3] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[4] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[5] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[6] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[21] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
#endif
        }
        virtual ~ Skill2069Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 254 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (877 + 35 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 63000 - 3000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               877 + 35 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (460 + 45 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (2.5 * skill->GetLevel () * skill->GetLevel () +
                                                                              45 * skill->GetLevel () + 300)), skill->GetLevel (),
                               12 * skill->GetLevel (), 4 + 2 * skill->GetLevel (), 63 - 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (4000 + 2000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.01001 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (4000 + 2000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.12 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccrithurt (1);
            return true;
        }
#endif
    };
}
#endif
