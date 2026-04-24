#ifndef __CPPGEN_GNET_SKILL4020
#define __CPPGEN_GNET_SKILL4020
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4020:public Skill
    {
      public:
        enum
        { SKILL_ID = 4020 };
          Skill4020 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4020Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetHasbuff (5441));
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHasbuff (4243));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4020Stub ():SkillStub (4020)
        {
            occupation = 58;
            name = L"Á«»ª¡¤Ïè";
            nativename = "Á«»ª¡¤Ïè";
            icon = "Á«»ª¡¤Ïè.dds";
            maxlevel = 8;
            maxlearn = 6;
            type = 4;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
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
            talent[0] = 4156;
            talent_size = 1;
            action[0] = "ÐùÔ¯_·ÉÐÐ";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ÐùÔ¯_·ÉÐÐ";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÐùÔ¯_Æï³Ë_·ÉÐÐ";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ÐùÔ¯_Æï³Ë_·ÉÐÐ";
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
#endif
        }
        virtual ~ Skill4020Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4019 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 500;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 90, 94, 98, 102, 106, 110 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 6 + skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetValue (15);
            skill->GetVictim ()->SetDodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()? 0 : 120));
            skill->GetVictim ()->SetTime (6000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (10 + skill->GetT0 ());
            skill->GetVictim ()->SetFly (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 ()? 0 : 120));
            skill->GetVictim ()->SetTime (6000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (10 + skill->GetT0 ());
            skill->GetVictim ()->SetAddspeed (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()? 120 : 0));
            skill->GetVictim ()->SetRatio (4243);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetVar2 ()? 10 : (5000 + 1000 * skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (4543);
            skill->GetVictim ()->SetDelaycast (1);
            return true;
        }
#endif
    };
}
#endif
