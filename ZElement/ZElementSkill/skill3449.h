#ifndef __CPPGEN_GNET_SKILL3449
#define __CPPGEN_GNET_SKILL3449
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3449:public Skill
    {
      public:
        enum
        { SKILL_ID = 3449 };
          Skill3449 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3449Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3449Stub ():SkillStub (3449)
        {
            occupation = 67;
            name = L"ÄùÅÌÖØÉú";
            nativename = "ÄùÅÌÖØÉú";
            icon = "ÄùÅÌÖØÉú.dds";
            maxlevel = 20;
            maxlearn = 6;
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
            skill_class = 9;
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
            talent[0] = 3420;
            talent[1] = 3423;
            talent[2] = 3759;
            talent_size = 3;
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
            action[13] = "";
            action[14] = "";
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
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
        virtual ~ Skill3449Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3448 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1600);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 100000 - 20000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 115, 117, 119, 121, 122, 122 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format, 2 * skill->GetLevel (), 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetAmount (2 * skill->GetLevel ());
            skill->GetVictim ()->SetCleardebuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (9999));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetAloof (1);
            skill->GetVictim ()->SetTime (6000 + 3000 * skill->GetT1 ());
            skill->GetVictim ()->SetValue (5 * skill->GetLevel ());
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
