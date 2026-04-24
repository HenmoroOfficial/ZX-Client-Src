#ifndef __CPPGEN_GNET_SKILL3874
#define __CPPGEN_GNET_SKILL3874
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3874:public Skill
    {
      public:
        enum
        { SKILL_ID = 3874 };
          Skill3874 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3874Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3874Stub ():SkillStub (3874)
        {
            occupation = 151;
            name = L"ÁúÌÚËÄº£";
            nativename = "ÁúÌÚËÄº£";
            icon = "ĞÂ´º×°Ê±×°¼¼ÄÜ.dds";
            maxlevel = 1;
            maxlearn = 10;
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
            action[0] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[1] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[2] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[3] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[4] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[5] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[6] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[7] = "ÊŞÉñ_Áú¹â";
            action[8] = "ºüÑı_°²ÈçÉ½";
            action[9] = "0";
            action[10] = "0";
            action[11] = "ÊŞÉñ_Áú¹â";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[21] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[22] = "ÊŞÉñ_Æï³Ë_Áú¹â";
            action[23] = "ºüÑı_Æï³Ë_°²ÈçÉ½";
            action[24] = "0";
            action[25] = "0";
            action[26] = "ÊŞÉñ_Æï³Ë_Áú¹â";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
#endif
        }
        virtual ~ Skill3874Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900000 - (skill->GetPlayer ()->GetSkilllevel (4730) == 1 ? 180000 : 0);
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
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
            return _snwprintf (buffer, length, format, 0, skill->GetPlayer ()->GetSkilllevel (4730) > 0 ? table.Find (38743) : table.Find (38744));

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetAmount (2 + (skill->GetPlayer ()->GetSkilllevel (4730) == 1 ? 1 : 0));
            skill->GetVictim ()->SetCleardebuff (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (25000);
            skill->GetVictim ()->SetRatio (0.4);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetIncskillaccu (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncdrugeffect (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (900000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (70);
            skill->GetVictim ()->SetAdddefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (200000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (20000 + (skill->GetPlayer ()->GetSkilllevel (4730) == 1 ? 10000 : 0));
            skill->GetVictim ()->SetMpgen (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
