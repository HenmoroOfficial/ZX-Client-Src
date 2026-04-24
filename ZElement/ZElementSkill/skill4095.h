#ifndef __CPPGEN_GNET_SKILL4095
#define __CPPGEN_GNET_SKILL4095
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4095:public Skill
    {
      public:
        enum
        { SKILL_ID = 4095 };
          Skill4095 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4095Stub:public SkillStub
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
                skill->SetRatio (1 + 0.04 * skill->GetLevel () + 0.03 * skill->GetT0 () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (11 - skill->GetLevel ())));
                skill->SetPlus (75 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4095Stub ():SkillStub (4095)
        {
            occupation = 130;
            name = L"±¦ËşÕòºÓÑı";
            nativename = "±¦ËşÕòºÓÑı";
            icon = "±¦ËşÕòºÓÑı.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            talent[0] = 687;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íµÀ_·¨±¦_ÁáççËş";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_ÁáççËş";
            action[2] = "¹íÍõ×Ú_·¨±¦_ÁáççËş";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_ÁáççËş";
            action[4] = "ÌìÒôËÂ_·¨±¦_ÁáççËş";
            action[5] = "¹íµÀ_·¨±¦_ÁáççËş";
            action[6] = "·ÙÏã¹È_·¨±¦_ÁáççËş";
            action[7] = "ÊŞÉñ_·¨±¦_ÁáççËş";
            action[8] = "ºüÑı_·¨±¦_ÁáççËş";
            action[9] = "";
            action[10] = "»³¹â_·¨±¦_ÁáççËş";
            action[11] = "Ì«ê»_·¨±¦_ÁáççËş";
            action[12] = "Ìì»ª_·¨±¦_ÁáççËş";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_ÁáççËş";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÁáççËş";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ÁáççËş";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÁáççËş";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÁáççËş";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_ÁáççËş";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÁáççËş";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_ÁáççËş";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_ÁáççËş";
            action[24] = "";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ÁáççËş";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_ÁáççËş";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÁáççËş";
            action[28] = "";
            action[29] = "";
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
        virtual ~ Skill4095Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (90 - 10 * skill->GetLevel ());
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
        float GetRadius (Skill * skill) const
        {
            return (float) (5);
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
                               90 - 10 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               4 * skill->GetLevel (),
                               75 * skill->GetLevel (),
                               11 - skill->GetLevel (),
                               15 + 5 * skill->GetLevel (),
                               30,
                               360 * skill->GetLevel () + 180,
                               40 * skill->GetLevel () + 40 * skill->GetLevel () * skill->GetLevel (),
                               25, 12, 2 * skill->GetLevel () * skill->GetLevel () + 8);

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
            skill->GetVictim ()->SetProbability (1.0 * ((15 + 5 * skill->GetLevel ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (360 * skill->GetLevel () + 180);
            skill->GetVictim ()->SetValue (40 * skill->GetLevel () + 40 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (360 * skill->GetLevel () + 180);
            skill->GetVictim ()->SetValue (40 * skill->GetLevel () + 40 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (25));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->
                SetValue (INT
                          ((2 * skill->GetLevel () * skill->GetLevel () +
                            8) * skill->GetPlayer ()->GetHp () / skill->GetPlayer ()->GetMaxhp () * 1.0));
            skill->GetVictim ()->SetAddattack (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (2);
        }
#endif
    };
}
#endif
