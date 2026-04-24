#ifndef __CPPGEN_GNET_SKILL3998
#define __CPPGEN_GNET_SKILL3998
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3998:public Skill
    {
      public:
        enum
        { SKILL_ID = 3998 };
          Skill3998 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3998Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 425;
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
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->
                    SetVar1 ((skill->GetPlayer ()->GetMp () <
                              skill->GetPlayer ()->GetMaxmp () / 10 ? skill->GetPlayer ()->GetMp () : skill->GetPlayer ()->GetMaxmp () / 10));
                skill->GetPlayer ()->SetMp (skill->GetPlayer ()->GetMp () - skill->GetPlayer ()->GetVar1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3998Stub ():SkillStub (3998)
        {
            occupation = 56;
            name = L"Àï½ç¡¤ö«ÔÆ";
            nativename = "Àï½ç¡¤ö«ÔÆ";
            icon = "Àï½ç¡¤ö«ÔÆ.dds";
            maxlevel = 9;
            maxlearn = 7;
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
            skill_class = 11;
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
            talent[0] = 4154;
            talent_size = 1;
            action[0] = "ÐùÔ¯_»¤¶Ü";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ÐùÔ¯_»¤¶Ü";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÐùÔ¯_Æï³Ë_»¤¶Ü";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ÐùÔ¯_Æï³Ë_»¤¶Ü";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill3998Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3997 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 875;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 20000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[7] = { 10, 12, 14, 16, 18, 20, 22 };
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
            return _snwprintf (buffer, length, format, skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio (4240);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 () * skill->GetLevel ());
            skill->GetVictim ()->SetDechurt2 (1);
            skill->GetVictim ()->SetTime (skill->GetT0 () > 0 ? 60000 : 0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * skill->GetT0 ());
            skill->GetVictim ()->SetAddmp (1);
            return true;
        }
#endif
    };
}
#endif
