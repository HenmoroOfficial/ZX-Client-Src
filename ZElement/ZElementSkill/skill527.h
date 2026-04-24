#ifndef __CPPGEN_GNET_SKILL527
#define __CPPGEN_GNET_SKILL527
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill527:public Skill
    {
      public:
        enum
        { SKILL_ID = 527 };
          Skill527 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill527Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill527Stub ():SkillStub (527)
        {
            occupation = 13;
            name = L"²Ô½ÙÕó";
            nativename = "²Ô½ÙÕó";
            icon = "²Ô½ÙÕó.dds";
            maxlevel = 18;
            maxlearn = 9;
            type = 2;
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
            action[0] = "¹íÍõ×Ú_²Ô½ÙÕó";
            action[1] = "¹íÍõ×Ú_²Ô½ÙÕó";
            action[2] = "¹íÍõ×Ú_²Ô½ÙÕó";
            action[3] = "¹íÍõ×Ú_²Ô½ÙÕó";
            action[4] = "¹íÍõ×Ú_²Ô½ÙÕó";
            action[5] = "¹íÍõ×Ú_²Ô½ÙÕó";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_²Ô½ÙÕó";
            action[16] = "¹íÍõ×Ú_Æï³Ë_²Ô½ÙÕó";
            action[17] = "¹íÍõ×Ú_Æï³Ë_²Ô½ÙÕó";
            action[18] = "¹íÍõ×Ú_Æï³Ë_²Ô½ÙÕó";
            action[19] = "¹íÍõ×Ú_Æï³Ë_²Ô½ÙÕó";
            action[20] = "¹íÍõ×Ú_Æï³Ë_²Ô½ÙÕó";
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
#endif
        }
        virtual ~ Skill527Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 523 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (235 - 5 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 125, 125, 125, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (8.5 + 0.5 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8.5 + 0.5 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (9);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               8.5 + 0.5 * skill->GetLevel (),
                               11 + 3 * skill->GetLevel (),
                               skill->GetLevel () / 2 * 1.5, 15 + skill->GetLevel (), skill->GetLevel () / 2 * 1.5, 235 - 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120 * int (skill->GetLevel () / 1.5)));
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetRatio (0.015 * int (skill->GetLevel () / 2));
              skill->GetVictim ()->SetBuffid (6);
              skill->GetVictim ()->SetIncfatalratio (1);
              skill->GetVictim ()->SetProbability (1.0 * (120));
              skill->GetVictim ()->SetTime (100);
              skill->GetVictim ()->SetBuffid (2);
              skill->GetVictim ()->SetAmount ((0.11 + 0.03 * skill->GetLevel ()) * skill->GetPlayer ()->GetVar1 ());
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetHpleak3 (1);
              return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetAmount (20);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetEvilaura (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * int (skill->GetLevel () / 1.5)));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetRatio (0.015 * int (skill->GetLevel () / 2));
              skill->GetVictim ()->SetBuffid (2);
              skill->GetVictim ()->SetIncskillaccu (1);
              return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15 + skill->GetLevel ());
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.3);
        }
#endif
    };
}
#endif
