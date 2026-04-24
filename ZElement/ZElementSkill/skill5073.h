#ifndef __CPPGEN_GNET_SKILL5073
#define __CPPGEN_GNET_SKILL5073
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5073:public Skill
    {
      public:
        enum
        { SKILL_ID = 5073 };
          Skill5073 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5073Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
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
                return 1600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetMp (0);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5073Stub ():SkillStub (5073)
        {
            occupation = 128;
            name = L"ÄùÅÌÖØÉú";
            nativename = "ÄùÅÌÖØÉú";
            icon = "ÄùÅÌÖØÉú.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
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
            action[0] = "·ÙÏã¹È_ÄùÅÌ»ØÌì";
            action[1] = "·ÙÏã¹È_ÄùÅÌ»ØÌì";
            action[2] = "·ÙÏã¹È_ÄùÅÌ»ØÌì";
            action[3] = "·ÙÏã¹È_ÄùÅÌ»ØÌì";
            action[4] = "·ÙÏã¹È_ÄùÅÌ»ØÌì";
            action[5] = "·ÙÏã¹È_ÄùÅÌ»ØÌì";
            action[6] = "·ÙÏã¹È_ÄùÅÌ»ØÌì";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÄùÅÌ»ØÌì";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÄùÅÌ»ØÌì";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÄùÅÌ»ØÌì";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÄùÅÌ»ØÌì";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÄùÅÌ»ØÌì";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÄùÅÌ»ØÌì";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÄùÅÌ»ØÌì";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
            action[30] = "¼¼ÄÜ¹¥»÷2";
            rangetype = 5;
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
        virtual ~ Skill5073Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetLevel () * 10);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 100000 - 20000 * 2;
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
            return _snwprintf (buffer, length, format, 2 * 6, 5 * 6, 5 * 4);

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
            skill->GetVictim ()->SetAmount (2 * 6);
            skill->GetVictim ()->SetCleardebuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncattack (1);
            skill->GetVictim ()->SetTime (6000 + 3000 * 2);
            skill->GetVictim ()->SetValue (5 * 6);
            skill->GetVictim ()->SetBlessed (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
#endif
    };
}
#endif
