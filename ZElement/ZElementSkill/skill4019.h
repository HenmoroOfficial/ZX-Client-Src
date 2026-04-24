#ifndef __CPPGEN_GNET_SKILL4019
#define __CPPGEN_GNET_SKILL4019
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4019:public Skill
    {
      public:
        enum
        { SKILL_ID = 4019 };
          Skill4019 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4019Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHasbuff (4241));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4019Stub ():SkillStub (4019)
        {
            occupation = 58;
            name = L"Á«»ª¡¤Ë²";
            nativename = "Á«»ª¡¤Ë²";
            icon = "Á«»ª¡¤Ë².dds";
            maxlevel = 8;
            maxlearn = 6;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 3;
            chargedist = 20;
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
            talent[0] = 4161;
            talent[1] = 4160;
            talent_size = 2;
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
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
#endif
        }
        virtual ~ Skill4019Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4018 };
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
            static int array[6] = { 80, 84, 88, 92, 96, 100 };
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
            return _snwprintf (buffer, length, format, 20, 65 - 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () * 120));
            skill->GetVictim ()->SetTime (3100);
            skill->GetVictim ()->SetRatio (0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskilldodge (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (((skill->GetPlayer ()->GetVar1 () == 0) && (skill->GetT1 () > 0)) ? 10 * skill->GetLevel () : 0));
            skill->GetVictim ()->SetAmount (skill->GetT1 ());
            skill->GetVictim ()->SetCleardebuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()? 120 : 0));
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetUnsummonclone (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetVar1 ()? 10 : 6000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (4544);
            skill->GetVictim ()->SetDelaycast (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()? 0 : 120));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (54786);
            skill->GetVictim ()->SetSummonclone (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()? 0 : 120));
            skill->GetVictim ()->SetTime (6200);
            skill->GetVictim ()->SetRecordpos (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()? 120 : 0));
            skill->GetVictim ()->SetReturnpos (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()? 120 : 0));
            skill->GetVictim ()->SetRatio (4241);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            return true;
        }
#endif
    };
}
#endif
