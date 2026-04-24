#ifndef __CPPGEN_GNET_SKILL4962
#define __CPPGEN_GNET_SKILL4962
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4962:public Skill
    {
      public:
        enum
        { SKILL_ID = 4962 };
          Skill4962 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4962Stub:public SkillStub
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
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (12 - skill->GetLevel ())));
                skill->SetPlus (15 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4962Stub ():SkillStub (4962)
        {
            occupation = 130;
            name = L"³Ğ¡¤½Ù×ªÁúÒ÷";
            nativename = "³Ğ¡¤½Ù×ªÁúÒ÷";
            icon = "½Ù×ªÒ÷.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
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
            action[0] = "Ìì»ª_·¨±¦_¾ÅÒôÂİ";
            action[1] = "01";
            action[2] = "01";
            action[3] = "01";
            action[4] = "01";
            action[5] = "01";
            action[6] = "01";
            action[7] = "01";
            action[8] = "01";
            action[9] = "01";
            action[10] = "01";
            action[11] = "01";
            action[12] = "Ìì»ª_·¨±¦_¾ÅÒôÂİ";
            action[13] = "01";
            action[14] = "Ó¢ÕĞ_·¨±¦_×·»ê";
            action[15] = "Ìì»ª_Æï³Ë_·¨±¦_¾ÅÒôÂİ";
            action[16] = "01";
            action[17] = "01";
            action[18] = "01";
            action[19] = "01";
            action[20] = "01";
            action[21] = "01";
            action[22] = "01";
            action[23] = "01";
            action[24] = "01";
            action[25] = "01";
            action[26] = "01";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_¾ÅÒôÂİ";
            action[28] = "01";
            action[29] = "Ó¢ÕĞ_Æï³Ë_·¨±¦_×·»ê";
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
        virtual ~ Skill4962Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (63 - 3 * skill->GetLevel ());
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               63 - 3 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               15 * skill->GetLevel (),
                               12 - skill->GetLevel (),
                               15 + 2 * skill->GetLevel (),
                               5 + skill->GetLevel (),
                               5 * skill->GetLevel (), 10 * skill->GetLevel (), 5 + skill->GetLevel (), 9 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (15 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (1000 * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (0.01 * 5 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecaccuracy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (10 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (1000 * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (9 * skill->GetLevel ());
            skill->GetVictim ()->SetAddattack (1);
            return true;
        }
#endif
    };
}
#endif
