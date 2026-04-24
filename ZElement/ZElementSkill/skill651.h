#ifndef __CPPGEN_GNET_SKILL651
#define __CPPGEN_GNET_SKILL651
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill651:public Skill
    {
      public:
        enum
        { SKILL_ID = 651 };
          Skill651 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill651Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetPlus (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill651Stub ():SkillStub (651)
        {
            occupation = 1;
            name = L"Î©ÎÒ";
            nativename = "Î©ÎÒ";
            icon = "Î©ÎÒ¶À×ð.dds";
            maxlevel = 6;
            maxlearn = 4;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            talent_type = 1;
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
            action[0] = "¹íÍõ×Ú_Î©ÎÒ¶À×ð";
            action[1] = "¹íÍõ×Ú_Î©ÎÒ¶À×ð";
            action[2] = "¹íÍõ×Ú_Î©ÎÒ¶À×ð";
            action[3] = "¹íÍõ×Ú_Î©ÎÒ¶À×ð";
            action[4] = "¹íÍõ×Ú_Î©ÎÒ¶À×ð";
            action[5] = "¹íÍõ×Ú_Î©ÎÒ¶À×ð";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Î©ÎÒ¶À×ð";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Î©ÎÒ¶À×ð";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Î©ÎÒ¶À×ð";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Î©ÎÒ¶À×ð";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Î©ÎÒ¶À×ð";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Î©ÎÒ¶À×ð";
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
            doenchant = false;
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
        virtual ~ Skill651Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 648 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 8);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (20 * skill->GetLevel () + 892 - 0.44 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 36000 - 4000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[4] = { 120, 125, 130, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10 + 2 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (6 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5 + 2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (20 * skill->GetLevel () + 892 - 0.44 * skill->GetLevel () * skill->GetLevel ()),
                               25 + skill->GetLevel (),
                               9 + 2 * skill->GetLevel (),
                               INT (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()),
                               4, skill->GetLevel (), 36 - 4 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4500);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetAddattack (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25 + skill->GetLevel ());
        }
#endif
    };
}
#endif
